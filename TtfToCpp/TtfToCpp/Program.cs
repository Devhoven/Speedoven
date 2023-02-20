#pragma warning disable CA1416 // Validate platform compatibility

using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

Bitmap GenerateBitmapFont(string fontFilePath, int fontWidth, string characters)
{
    // Load the TTF file into a PrivateFontCollection
    PrivateFontCollection privateFontCollection = new PrivateFontCollection();
    privateFontCollection.AddFontFile(fontFilePath);

    // Create a new Font using the first font in the PrivateFontCollection
    FontFamily fontFamily = privateFontCollection.Families[0];
    Font font = new Font(fontFamily, fontWidth, FontStyle.Regular, GraphicsUnit.Pixel);

    // Create a new Bitmap for the font
    int bitmapWidth = (fontWidth * characters.Length);
    int bitmapHeight = (int)(fontWidth * 1.5);
    Bitmap bitmap = new Bitmap(bitmapWidth, bitmapHeight, PixelFormat.Format32bppArgb);

    // Draw each character onto the bitmap
    using (Graphics graphics = Graphics.FromImage(bitmap))
    {
        graphics.Clear(Color.White);
        graphics.TextRenderingHint = TextRenderingHint.SingleBitPerPixel;
        graphics.SmoothingMode = SmoothingMode.HighQuality;
        graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
        graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

        for (int i = 0; i < characters.Length; i++)
        {
            PointF characterPosition = new PointF(fontWidth * i, 0);

            graphics.DrawString(characters[i].ToString(), font, Brushes.Black, characterPosition);
        }
    }

    return bitmap;
}


void GenerateFontHeader(string fontFilePath, int fontWidth, AsciiRange asciiRange, string fontName)
{
    if (fontWidth % 8 != 0)
        throw new Exception("FontWidth has to be divisible by 8");

    string characters = asciiRange.ToString();

    // Generates a bitmap font
    Bitmap bmpFont = GenerateBitmapFont(fontFilePath, fontWidth, characters);
    int fontHeight = bmpFont.Height;
    int newFontWidth = fontWidth / 8;

    // A byte array which practically contains a compressed binary representation of the bmp font
    byte[] bmpArr = new byte[fontHeight * bmpFont.Width / 8];
    for (int i = 0; i < characters.Length; i++)
    {
        for (int y = 0; y < fontHeight; y++)
        {
            for (int x = 0; x < fontWidth; x++)
            {
                Color pixelColor = bmpFont.GetPixel(x + fontWidth * i, y);
                if (IsWhite(pixelColor))
                    bmpArr[i * newFontWidth * fontHeight + (x + y * fontWidth) / 8] |= (byte)(0x01 << (x % 8));
                else
                    bmpArr[i * newFontWidth * fontHeight + (x + y * fontWidth) / 8] &= (byte)(~(0x01 << (x % 8)));
            }
        }
    }

    // Returns true if all of the color components are above a threshold
    bool IsWhite(Color color)
    {
        const byte THRESHOLD = 100;
        return color.R > THRESHOLD && color.G > THRESHOLD && color.B > THRESHOLD;
    }

    // Generating text for a header file in which the font is encoded in a uint8_t c-array 
    StringBuilder fontHeaderBuilder = new StringBuilder();
    fontHeaderBuilder.Append($"#include \"Fonts.h\" \n" +
                             $"\n" +
                             $"const uint8_t Font{fontWidth}Table[] = \n" +
                             $"{{\n" +
                             $"\t");

    for (int i = 0; i < bmpArr.Length; i++)
    {
        // If bmpArr[i] is < 16, bmpArr[i].ToString("X") outputs 0xX and not 0x0X
        // This query fixes that formatting issue
        if (bmpArr[i] < 16)
            fontHeaderBuilder.Append("0x0");
        else
            fontHeaderBuilder.Append("0x");

        // Appends the current value in hex
        fontHeaderBuilder.Append(bmpArr[i].ToString("X"));
        fontHeaderBuilder.Append(", ");

        if (i == 0)
            continue;

        // This query is true after each image row of an character
        if ((i + 1) % newFontWidth == 0)
        {
            // It appends a representation of the encoded character as a comment 
            fontHeaderBuilder.Append(" // ");
            for (int j = 0; j < fontWidth; j++)
            {
                // Checks if the current pixel is white or black
                if ((bmpArr[i - (newFontWidth - 1 - (j / 8))] & (1 << j % 8)) != 0)
                    fontHeaderBuilder.Append('#');
                else
                    fontHeaderBuilder.Append(' ');
            }
            fontHeaderBuilder.Append("\n\t");
        }
        // Is true after each character and appends a linebreak after each one 
        if ((i + 1) % (bmpFont.Height * newFontWidth) == 0)
            fontHeaderBuilder.Append("\n\t");
    }
    // Removing the last linebreak
    fontHeaderBuilder.Remove(fontHeaderBuilder.Length - 2, 1);
    fontHeaderBuilder.Append("};\n\n");

    // Creates an instance of the Font-class
    fontHeaderBuilder.Append($"FONT Font{fontWidth} = \n" +
                            $"{{\n" +
                            $"\tFont{fontWidth}Table, \n" +
                            $"\t{fontWidth}, // Width \n" +
                            $"\t{fontHeight}, // Height\n" +
                            $"\t{asciiRange.Start}, // AsciiRange-Start \n" +
                            $"}};");

    File.WriteAllLines($"{fontName}{fontWidth}x{fontHeight}.h", new string[] {fontHeaderBuilder.ToString()});
}

GenerateFontHeader(@"C:\Users\veste\source\repos\TtfToCpp\TtfToCpp\NotoSansMonoRegular.ttf",
                   16,
                   new AsciiRange(' ', 'z'),
                   // new AsciiRange('0', '9'),
                   "NotoSans");

struct AsciiRange
{
    public int Start;
    // End is inclusive
    public int End;

    public AsciiRange(int start, int end)
    {
        Start = start;
        End = end;
    }

    // Generates all of the characters in the given ascii range
    public override string ToString()
    {
        StringBuilder charactersBuilder = new StringBuilder();
        for (int i = Start; i <= End; i++)
            charactersBuilder.Append((char)i);

        return charactersBuilder.ToString();
    }
}