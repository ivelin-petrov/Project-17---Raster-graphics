#ifndef PIXEL_H_
#define PIXEL_H_

#include <iostream>

class Pixel
{
  private:
    int red;
    int green;
    int blue;
  public:
    Pixel(int _r = 0, int _g = 0, int _b = 0);
    Pixel(const Pixel& other);
    Pixel& operator = (const Pixel& other);

    ~Pixel() {}

    int getRed() const;
    int getGreen() const;
    int getBlue() const;

    bool operator == (const Pixel& other);

  friend std::istream& operator >> (std::istream& in, Pixel& pixel);
  friend std::ostream& operator << (std::ostream& out, const Pixel& pixel);
};

#endif // PIXEL_H_