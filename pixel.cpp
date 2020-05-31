#include "pixel.h"

Pixel::Pixel(int _r, int _g, int _b) : red(_r), green(_g), blue(_b) {} 

Pixel::Pixel(const Pixel& other) : red(other.red), green(other.green), blue(other.blue) {}

Pixel& Pixel::operator = (const Pixel& other)
{
    if(this != &other)
    {
        red = other.red;
        green = other.green;
        blue = other.blue;
    }

    return *this;
}

int Pixel::getRed() const { return red; }
int Pixel::getGreen() const { return green; }
int Pixel::getBlue() const { return blue; }

bool Pixel::operator == (const Pixel& other)
{
    return ((red == other.red) && (green == other.green) && (blue == other.blue));
}

std::istream& operator >> (std::istream& in, Pixel& pixel)
{
    in >> pixel.red >> pixel.green >> pixel.blue;

    return in;
}

std::ostream& operator << (std::ostream& out, const Pixel& pixel)
{
    out << pixel.getRed() << " " << pixel.getGreen() << " " << pixel.getBlue() << "  ";

    return out;
}