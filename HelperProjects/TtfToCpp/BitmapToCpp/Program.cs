using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
#pragma warning disable CA1416 // Validate platform compatibility


// From: https://stackoverflow.com/a/24199315
Bitmap ResizeImage(Image image, int width, int height)
{
    var destRect = new Rectangle(0, 0, width, height);
    var destImage = new Bitmap(width, height);

    destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

    using (var graphics = Graphics.FromImage(destImage))
    {
        graphics.CompositingMode = CompositingMode.SourceCopy;
        graphics.CompositingQuality = CompositingQuality.HighQuality;
        graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
        graphics.SmoothingMode = SmoothingMode.HighQuality;
        graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

        using (var wrapMode = new ImageAttributes())
        {
            wrapMode.SetWrapMode(WrapMode.TileFlipXY);
            graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
        }
    }

    return destImage;
}


string GeneradeBmpHexCode(Bitmap bmp, string arrName)
{
    if (bmp.Width % 8 != 0)
        throw new Exception("FontWidth has to be divisible by 8");

    int height = bmp.Height;
    int width  = bmp.Width;
    int newWidth = width / 8;

    // A byte array which practically contains a compressed binary representation of the bmp font
    byte[] bmpArr = new byte[height * newWidth];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Color pixelColor = bmp.GetPixel(x, y);
            if (IsWhite(pixelColor))
                bmpArr[(x + y * width) / 8] |= (byte)(0x01 << (x % 8));
            else
                bmpArr[(x + y * width) / 8] &= (byte)(~(0x01 << (x % 8)));
        }
    }

    // Returns true if all of the color components are above a threshold
    bool IsWhite(Color color)
    {
        const byte THRESHOLD = 100;
        return color.A < THRESHOLD;
        //return color.R > THRESHOLD && color.G > THRESHOLD && color.B > THRESHOLD;
    }

    // Generating text for a header file in which the font is encoded in a uint8_t c-array 
    StringBuilder hextCodeBuilder = new StringBuilder();
    hextCodeBuilder.Append($"const uint8_t {arrName}Img[] = \n" +
                           $"{{\n" +
                           $"\t");

    for (int i = 0; i < bmpArr.Length; i++)
    {
        // If bmpArr[i] is < 16, bmpArr[i].ToString("X") outputs 0xX and not 0x0X
        // This query fixes that formatting issue
        if (bmpArr[i] < 16)
            hextCodeBuilder.Append("0x0");
        else
            hextCodeBuilder.Append("0x");

        // Appends the current value in hex
        hextCodeBuilder.Append(bmpArr[i].ToString("X"));
        hextCodeBuilder.Append(", ");

        if (i == 0)
            continue;


        // This query is true after each image row of an character
        if ((i + 1) % newWidth == 0)
        {
            // It appends a representation of the encoded character as a comment 
            hextCodeBuilder.Append(" // ");
            for (int j = 0; j < width; j++)
            {
                // Checks if the current pixel is white or black
                if ((bmpArr[i - (newWidth - 1 - (j / 8))] & (1 << j % 8)) != 0)
                    hextCodeBuilder.Append('#');
                else
                    hextCodeBuilder.Append(' ');
            }
            hextCodeBuilder.Append("\n\t");
        }

        if ((i + 1) % (newWidth * height) == 0)
            hextCodeBuilder.Append("\n\t");
    }

    // Removing the last linebreak
    hextCodeBuilder.Remove(hextCodeBuilder.Length - 3, 3);
    hextCodeBuilder.Append("};\n\n");

    return hextCodeBuilder.ToString();
}

string dirPath = "../../../NavigationArrows";

string[] pngNames = new string[] { "GoStraight", "Start", "Finish", "SlightLeft", "Left", 
                                   "SharpLeft", "SharpRight", "Right", "SlightRight", 
                                   "ForkRight", "ForkLeft", "UTurn",
                                   "RoundaboutExitLeft", "RoundaboutExitRight", 
                                   "RoundaboutCounterClockwiseExit1of1", "RoundaboutCounterClockwiseExit1of2", 
                                   "RoundaboutCounterClockwiseExit1of3", "RoundaboutCounterClockwiseExit2of2", 
                                   "RoundaboutCounterClockwiseExit2of3", "RoundaboutCounterClockwiseExit3of3", 
                                   "RoundaboutClockwiseExit1of1", "RoundaboutClockwiseExit1of2", 
                                   "RoundaboutClockwiseExit1of3", "RoundaboutClockwiseExit2of2", 
                                   "RoundaboutClockwiseExit2of3", "RoundaboutClockwiseExit3of3", 
                                   "RoundaboutFallback", "OutOfRoute" };

StringBuilder builder = new StringBuilder();

for (int i = 0; i < pngNames.Length; i++)
{
    Bitmap bmp = ResizeImage(Image.FromFile($"{dirPath}/{pngNames[i]}.png"), 96, 96);
    builder.Append(GeneradeBmpHexCode(bmp, pngNames[i]));
    builder.Append('\n');
}

File.WriteAllLines("Images.h", new string[] { builder.ToString() });