#ifndef SESSION_H__
#define SESSION_H__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include "pixel.h"

class Session{
  private:
    std::vector<std::string> commands;
    std::vector<std::string> images;
    int ID;

  public:
    Session(const std::vector<std::string> _commands = {},const std::vector<std::string> _images = {}, int _ID = 0);
    Session(const Session& other);
    Session& operator = (const Session& other);
    ~Session();

    int getID() const;
    std::string& getCommand(int i);
    std::string getCommand(int i) const;
    int getCommandsSize() const ;
    std::string& getImage(int i);
    std::string getImage(int i) const;
    int getImagesSize() const ;

    void addCommand(const std::string& command);
    void addImage(const std::string& image);

    void changeID(int _ID);

    bool foundCommand(const std::string& command);
    bool foundImage(const std::string& image);

    // information
    void session_info();

    void rotate_left(const std::string& image);
    void rotate_right(const std::string& image);

    void collage_horizontal(const std::string& image, const std::string& image2, const std::string& outimage);
    void collage_vertical(const std::string& image, const std::string& image2, const std::string& outimage);

    // transformations 
    void grayscale();
    void monochrome();
    void negative();
    void rotate(const std::string& direction); // <direction>
    void undo();
    void add(const std::string& image); // <image>
    void collage(const std::string& direction, const std::string& image1, 
                        const std::string& image2, const std::string& outimage); 
    // <direction> <image1> <image2>, the result is saved in <outimage>
};

bool correctImage(const std::string& image);
std::string getType(const std::string& image);
bool sameType(const std::string& image1, const std::string& image2);
std::string fileSize(const std::string& image);

std::vector<std::vector<int>> deserialize_matrix(const std::string& name);
std::vector<std::vector<Pixel>> deserialize_pixel_matrix(const std::string& name);

void serialize_matrix(const std::string& file, const std::vector<std::vector<int>>& matrix,
                      const std::string& file_type, size_t width, size_t height);

void serialize_matrix_with_max_value(const std::string& file, const std::vector<std::vector<int>>& matrix,
                      const std::string& file_type, size_t width, size_t height, int max);

void serialize_pixel_matrix_with_max_value(const std::string& file, const std::vector<std::vector<Pixel>>& matrix,
                      const std::string& file_type, size_t width, size_t height, int max);

#endif