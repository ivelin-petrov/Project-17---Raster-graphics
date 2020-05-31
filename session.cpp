#include "session.h"

Session::Session(const std::vector<std::string> _commands, const std::vector<std::string> _images, int _ID) 
    : commands(_commands), images(_images), ID(_ID) {}

Session::Session(const Session& other)
    : commands(other.commands), images(other.images), ID(other.ID) {}

Session& Session::operator = (const Session& other){
    if(this != &other)
    {
        commands = other.commands;
        images = other.images;
        ID = other.ID;
    }

    return *this;
}

Session::~Session() {}

bool correctImage(const std::string& image){
    bool usable = true;
    int j = 0;
    for(int i = 0; i < image.size(); ++i){
        if(image[i] == '.'){
            j = i + 1;
            break;
        }
        if((image[i] < 'a' || image[i] > 'z') && (image[i] < 'A' || image[i] > 'Z') 
         && image[i] != '_' && (image[i] < '0' || image[i] > '9')){
            usable = false;
            break;
        }
    }
    
    std::string ending = image.substr(j);
    if(usable == true){    //pbm pgm ppm
        if(ending != "pbm" && ending != "pgm" && ending != "ppm")
        {
            usable = false;
        }        
    }

    if(image[0] == '.'){
        usable = false;
    }

    return usable;
}

void Session::addCommand(const std::string& command){
    commands.push_back(command);
}

void Session::addImage(const std::string& image){
    if(correctImage(image) == 1){
        images.push_back(image);
        //std::cout << "Image '" << image << "' added." << std::endl;
    }else{
        std::cout << "Could not add this image, wrong type." << std::endl;
    }
}

void Session::add(const std::string& image){
    if(correctImage(image) == 1){
        images.push_back(image);
        std::cout << "Image '" << image << "' added." << std::endl;
    }else{
        std::cout << "Could not add this image, wrong type." << std::endl;
    }
}

void Session::changeID(int _ID){ ID = _ID; }

int Session::getID() const{ return ID; }

std::string& Session::getCommand(int i) { return commands[i]; }
std::string Session::getCommand(int i) const { return commands[i]; }

int Session::getCommandsSize() const { return commands.size(); }

std::string& Session::getImage(int i){ return images[i]; }
std::string Session::getImage(int i) const { return images[i]; }

int Session::getImagesSize() const { return images.size(); }

bool Session::foundCommand(const std::string& command){
    bool found = false;
    for(size_t i = 0; i < commands.size(); ++i)
    {
        if(commands[i] == command){
            found = true;
            break;
        }
    }
    
    return found;
}

bool Session::foundImage(const std::string& image){
    bool found = false;
    for(size_t i = 0; i < images.size(); ++i)
    {
        if(images[i] == image){
            found = true;
            break;
        }
    }

    return found;
}

std::string getType(const std::string& image){
    int j = 0;
    for(int i = 0; i < image.size(); ++i)
    {
        if(image[i] == '.')
        {
            j = i;
            break;
        }
    }

    return image.substr(j);
}

bool sameType(const std::string& image1, const std::string& image2){
    return (getType(image1) == getType(image2));
}

std::string fileSize(const std::string& image)
{
    std::ifstream ifile(image);

    std::string line;
    if(ifile.good())
    {
        std::string type, max;
        std::getline(ifile, type);   // gets the first line of the file with the type
        std::getline(ifile, line);   // gets the second line of the file with the size of the matrix (image)

        if(type != "P1" && type != "P4")
        {
            std::getline(ifile, max); 
            ifile.close();

            return (line + " " + max);
        }
    }else{
        std::cout << "Error while opening the file." << std::endl;
    }
    ifile.close();
    
    return line;
}

void Session::session_info()
{
    std::cout << std::endl;
    std::cout << "Session ID: " << ID << std::endl;
    std::cout << "Name of images in the session: ";
    for(size_t i = 0; i < images.size(); ++i)
    {
        std::cout << images[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Pending transformations: ";
    for(size_t i = 0; i < commands.size(); ++i)
    {
        std::cout << commands[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<std::vector<int>> deserialize_matrix(const std::string& name)
{
    std::ifstream ifile(name);

    std::string file_type;
    size_t width, height; 

    std::vector<std::vector<int>> matrix(height);

    ifile >> file_type;
    ifile >> width >> height;

    if(file_type == "P1" || file_type == "P4")
    {
        int element;
        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> element;
                matrix[i].push_back(element);
            }
        }
    }
    else if(file_type == "P2" || file_type == "P5")
    {
        int element;
        int maxShade;
        ifile >> maxShade;

        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> element;
                matrix[i].push_back(element);
            }
        }
    }
    else
    {
        std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
    }
    ifile.close();

    return matrix;
}

std::vector<std::vector<Pixel>> deserialize_pixel_matrix(const std::string& name)
{
    std::ifstream ifile(name);

    std::string file_type;
    size_t width, height; 

    std::vector<std::vector<Pixel>> pixel_matrix(height);

    ifile >> file_type;
    ifile >> width >> height;

    if(file_type == "P3" || file_type == "P6")
    {
        Pixel pixel;
        int maxColour;   
        ifile >> maxColour;

        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> pixel;
                pixel_matrix[i].push_back(pixel);
            }
        }
    }
    else
    {
        std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
    }
    ifile.close();

    return pixel_matrix;
}

void Session::grayscale(){
    for(size_t i = 0; i < images.size(); ++i)
    {
        std::ifstream current_ifile(images[i]);

        std::string file_type;
        current_ifile >> file_type;

        if(file_type == "P1" || file_type == "P4")
        {
            std::cout << "'" << images[i] << "' is a black & white image. Doesn't need modifications." << std::endl;
        }
        else if(file_type == "P2" || file_type == "P5")
        {
            std::cout << "'" << images[i] << "' is already gray-shaded. Doesn't need modifications." << std::endl;
        }
        else if(file_type == "P3" || file_type == "P6")
        {
            size_t width, height; 
            int maxColour; // usually maxColour = 255

            current_ifile >> width >> height >> maxColour;
            std::vector<std::vector<Pixel>> matrix(height);

            Pixel pixel;
            for(size_t i = 0; i < height; ++i)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    current_ifile >> pixel;

                    if(pixel.getRed() == 0 && pixel.getGreen() == 0 && pixel.getBlue() == 0)
                    {
                        matrix[i].push_back(pixel); // black pixel
                    }
                    else if(pixel.getRed() == maxColour && pixel.getGreen() == maxColour && pixel.getBlue() == maxColour)
                    {
                        matrix[i].push_back(pixel); // whitest pixel 
                                              // if maxColour = 255 => the pixel is white
                    }
                    else
                    {
                        Pixel custom_pixel(((maxColour / 2) + 1), ((maxColour / 2) + 1), ((maxColour / 2) + 1));
                        matrix[i].push_back(custom_pixel);
                                              // if maxColour = 255 => gray is (128, 128, 128)
                    }         
                }
            }
            // coloured images are turned into gray-shaded ones  
            current_ifile.close();

            // saving changes

            serialize_pixel_matrix_with_max_value(images[i], matrix, file_type, width, height, maxColour);
            
            // all done for this image  
        }
        else
        {
            std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
        }
    }
}

void Session::monochrome(){
    for(size_t i = 0; i < images.size(); ++i)
    {
        std::ifstream current_ifile(images[i]);

        std::string file_type;
        std::getline(current_ifile, file_type);

        if(file_type == "P1" || file_type == "P4")
        {
            std::cout << "'" << images[i] << "' is a black & white image. Doesn't need modifications." << std::endl;
        }
        else if(file_type == "P2" || file_type == "P5")
        {
            size_t width, height; 
            int maxShade;

            current_ifile >> width >> height >> maxShade;
            std::vector<std::vector<int>> matrix(height);

            int element;
            for(size_t i = 0; i < height; ++i)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    current_ifile >> element;

                    if(element != 0){
                        // element = maxShade; // makes the element white(maxShade)

                        matrix[i].push_back(maxShade); // if element == maxShade => element is black
                    }else if(element == 0){
                        matrix[i].push_back(element); 
                    }
                }
            }

            // gray-shaded images are turned into black & white ones
            current_ifile.close();

            // saving changes

            serialize_matrix_with_max_value(images[i], matrix, file_type, width, height, maxShade);
            
            // all done for this image
        }
        else if(file_type == "P3" || file_type == "P6")
        {
            size_t width, height; 
            int maxColour;  // usually maxColour = 255

            current_ifile >> width >> height >> maxColour;
            std::vector<std::vector<Pixel>> matrix(height);

            Pixel pixel;
            for(size_t i = 0; i < height; ++i)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    current_ifile >> pixel;

                    if(pixel.getRed() == 0 && pixel.getGreen() == 0 && pixel.getBlue() == 0){
                        matrix[i].push_back(pixel);    // this pixel is black
                    }else{
                        // pixel = (maxColour, maxColour, maxColour); // makes the pixel the whitest
                                                                      // if maxColour = 255 => the pixel is white
                        Pixel max_pixel(maxColour, maxColour, maxColour);
                        matrix[i].push_back(max_pixel);
                    }
                }
            }
            // coloured images are turned into black & white ones  
            current_ifile.close();

            // saving changes

            serialize_pixel_matrix_with_max_value(images[i], matrix, file_type, width, height, maxColour);

            // all done for this image  
        }
        else
        {
            std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
        }
    }
}   

void Session::negative(){
    for(size_t i = 0; i < images.size(); ++i)
    {
        std::ifstream current_ifile(images[i]);

        std::string file_type;
        std::getline(current_ifile, file_type);


        if(file_type == "P1" || file_type == "P4")
        {
            size_t width, height; 

            current_ifile >> width >> height;
            std::vector<std::vector<int>> matrix(height);

            int element;
            for(size_t i = 0; i < height; ++i)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    current_ifile >> element;

                    if(element == 0){
                        // element == 0 is black

                        matrix[i].push_back(1); // so we turn it white
                    }else if(element == 1){
                        // element == 1 is white

                        matrix[i].push_back(0); // so we turn it black
                    }
                }
            }

            // black & white images are turned negative
            current_ifile.close();

            // saving changes

            serialize_matrix(images[i], matrix, file_type, width, height);

            // all done for this image
            
        }
        else if(file_type == "P2" || file_type == "P5")
        {
            size_t width, height; 
            int maxShade;

            current_ifile >> width >> height >> maxShade;
            std::vector<std::vector<int>> matrix(height);

            int element;
            for(size_t i = 0; i < height; ++i)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    current_ifile >> element;
                    // we turn the colours around
                    // with the help of the maxShade value
                    
                    matrix[i].push_back(maxShade - element);
                }
            }

            // gray-shaded images are turned negative
            current_ifile.close();

            // saving changes

            serialize_matrix_with_max_value(images[i], matrix, file_type, width, height, maxShade);

            // all done for this image
        }
        else if(file_type == "P3" || file_type == "P6")
        {
            size_t width, height; 
            int maxColour;  // usually maxColour = 255

            current_ifile >> width >> height >> maxColour;
            std::vector<std::vector<Pixel>> matrix(height);

            Pixel pixel;
            for(size_t i = 0; i < height; ++i)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    current_ifile >> pixel;

                    // we turn pixels and their colours around
                    // with the help of the maxColour value

                    Pixel negative_pixel((maxColour - pixel.getRed()), 
                                        (maxColour - pixel.getGreen()), 
                                        (maxColour - pixel.getBlue()));
                    matrix[i].push_back(negative_pixel);
                }
            }
            // coloured images are turned negative
            current_ifile.close();

            // saving changes

           serialize_pixel_matrix_with_max_value(images[i], matrix, file_type, width, height, maxColour);

            // all done for this image  
        }
        else
        {
            std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
        }
    }
}

void Session::rotate_left(const std::string& image){
    std::ifstream ifile(image);

    std::string file_type;
    size_t width, height; 

    ifile >> file_type;
    ifile >> width >> height;

    if(file_type == "P1" || file_type == "P4")
    {
        std::vector<std::vector<int>> matrix(height);
        int element;
        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> element;
                matrix[i].push_back(element);
            }
        }
        

        // saving changes

        std::ofstream ofile(image);
        ofile << file_type << std::endl;
        ofile << height << " " << width << std::endl; // we swap width and height
        for(size_t k = 0; k < width; ++k)
        {
            for(size_t p = 0; p < height; ++p)
            {
                ofile << matrix[p][width - k - 1] << " ";
            }
            ofile << std::endl;
        }
        ofile.close();
        // all done for this image  
    }
    else if(file_type == "P2" || file_type == "P5")
    {
        int maxShade;
        ifile >> maxShade;

        std::vector<std::vector<int>> matrix(height);
        int element;
        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> element;
                matrix[i].push_back(element);
            }
        }

        // saving changes

        std::ofstream ofile(image);
        ofile << file_type << std::endl;
        ofile << height << " " << width << std::endl; // we swap width and height
        ofile << maxShade << std::endl;
        for(size_t k = 0; k < width; ++k)
        {
            for(size_t p = 0; p < height; ++p)
            {
                ofile << matrix[p][width - k - 1] << " ";
            }
            ofile << std::endl;
        }
        ofile.close();
        // all done for this image  
    }
    else if(file_type == "P3" || file_type == "P6")
    {
        int maxColour;   
        ifile >> maxColour;

        std::vector<std::vector<Pixel>> matrix(height);
        Pixel pixel;
        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> pixel;
                matrix[i].push_back(pixel);
            }
        }

        // saving changes

        std::ofstream ofile(image);
        ofile << file_type << std::endl;
        ofile << height << " " << width << std::endl; // we swap width and height
        ofile << maxColour << std::endl;
        for(size_t k = 0; k < width; ++k)
        {
            for(size_t p = 0; p < height; ++p)
            {
                ofile << matrix[p][width - k - 1] << " ";
            }
            ofile << std::endl;
        }
        ofile.close();
        // all done for this image  
    }
    else
    {
        std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
    }
}

void Session::rotate_right(const std::string& image){
    std::ifstream ifile(image);

    std::string file_type;
    size_t width, height; 

    ifile >> file_type;
    ifile >> width >> height;

    if(file_type == "P1" || file_type == "P4")
    {
        std::vector<std::vector<int>> matrix(height);
        int element;
        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> element;
                matrix[i].push_back(element);
            }
        }

        // saving changes

        std::ofstream ofile(image);
        ofile << file_type << std::endl;
        ofile << height << " " << width << std::endl; // we swap width and height
        for(size_t k = 0; k < width; ++k)
        {
            for(size_t p = 0; p < height; ++p)
            {
                ofile << matrix[height - p - 1][k] << " ";
            }
            ofile << std::endl;
        }
        ofile.close();
        // all done for this image  
    }
    else if(file_type == "P2" || file_type == "P5")
    {
        int maxShade;
        ifile >> maxShade;

        std::vector<std::vector<int>> matrix(height);
        int element;
        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> element;
                matrix[i].push_back(element);
            }
        }

        // saving changes

        std::ofstream ofile(image);
        ofile << file_type << std::endl;
        ofile << height << " " << width << std::endl; // we swap width and height
        ofile << maxShade << std::endl;
        for(size_t k = 0; k < width; ++k)
        {
            for(size_t p = 0; p < height; ++p)
            {
                ofile << matrix[height - p - 1][k] << " ";
            }
            ofile << std::endl;
        }
        ofile.close();
        // all done for this image  
    }
    else if(file_type == "P3" || file_type == "P6")
    {
        int maxColour;   
        ifile >> maxColour;

        std::vector<std::vector<Pixel>> matrix(height);
        Pixel pixel;
        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width; ++j)
            {
                ifile >> pixel;
                matrix[i].push_back(pixel);
            }
        }

        // saving changes

        std::ofstream ofile(image);
        ofile << file_type << std::endl;
        ofile << height << " " << width << std::endl; // we swap width and height
        ofile << maxColour << std::endl;
        for(size_t k = 0; k < width; ++k)
        {
            for(size_t p = 0; p < height; ++p)
            {
                ofile << matrix[height - p - 1][k] << " ";
            }
            ofile << std::endl;
        }
        ofile.close();
        // all done for this image  
    }
    else
    {
        std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
    }
}

void Session::rotate(const std::string& direction){
    if(direction == "left") // rotating all images in the left direction by 90 degrees
    {
        for(size_t i = 0; i < images.size(); ++i)
        {
            rotate_left(images[i]);
        }
    }
    else if(direction == "right") // rotating all images in the right direction by 90 degrees
    {
        for(size_t i = 0; i < images.size(); ++i)
        {
            rotate_right(images[i]);
        }
    }
}

void Session::undo(){
    if(commands.size() == 0){
        std::cout << "No commands to undo." << std::endl;
    }else{
        commands.pop_back(); // removes the last command
    }
}

void serialize_matrix(const std::string& file, const std::vector<std::vector<int>>& matrix,
                      const std::string& file_type, size_t width, size_t height)
{
    std::ofstream ofile(file);

    ofile << file_type << std::endl;
    ofile << width << " " << height << std::endl;

    for(size_t k = 0; k < height; ++k)
    {
        for(size_t p = 0; p < width; ++p)
        {
            ofile << matrix[k][p] << " ";
        }
        ofile << std::endl;
    }

    ofile.close(); 
}

void serialize_matrix_with_max_value(const std::string& file, const std::vector<std::vector<int>>& matrix,
                      const std::string& file_type, size_t width, size_t height, int max)
{
    std::ofstream ofile(file);

    ofile << file_type << std::endl;
    ofile << width << " " << height << std::endl;
    ofile << max << std::endl;

    for(size_t k = 0; k < height; ++k)
    {
        for(size_t p = 0; p < width; ++p)
        {
            ofile << matrix[k][p] << " ";
        }
        ofile << std::endl;
    }
    
    ofile.close(); 
}

void serialize_pixel_matrix_with_max_value(const std::string& file, const std::vector<std::vector<Pixel>>& matrix,
                                    const std::string& file_type, size_t width, size_t height, int max)
{
    std::ofstream ofile(file);

    ofile << file_type << std::endl;
    ofile << width << " " << height << std::endl;
    ofile << max << std::endl;

    for(size_t k = 0; k < height; ++k)
    {
        for(size_t p = 0; p < width; ++p)
        {
            ofile << matrix[k][p] << " ";
        }
        ofile << std::endl;
    }
    
    ofile.close(); 
}

void Session::collage_horizontal(const std::string& image1, const std::string& image2, const std::string& outimage)
{
    std::ifstream image1_ifile(image1);
    std::ifstream image2_ifile(image2);

    std::string file_type;
    size_t width, height; 

    image1_ifile >> file_type;
    image1_ifile >> width >> height;

    image2_ifile >> file_type;
    image2_ifile >> width >> height;
    

    if(file_type == "P1" || file_type == "P4")
    {
        //std::vector<std::vector<int>> matrix1 = deserialize_matrix(image1);
        //std::vector<std::vector<int>> matrix2 = deserialize_matrix(image2);
        std::vector<std::vector<int>> matrix1(height);
        std::vector<std::vector<int>> matrix2(height);
        int element;
        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image1_ifile >> element;
                matrix1[i].push_back(element);
            }
        }

        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image2_ifile >> element;
                matrix2[i].push_back(element);
            }
        }
        image1_ifile.close();
        image2_ifile.close();

        std::vector<std::vector<int>> outmatrix(height);

        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width * 2; ++j) // horizontal
            {
                if(j < width)
                {
                    outmatrix[i].push_back(matrix1[i][j]); 

                }else if(j >= width){
                    outmatrix[i].push_back(matrix2[i][j - width]);
                }
            }
        }

        serialize_matrix(outimage, outmatrix, file_type, width * 2, height);
    }
    else if(file_type == "P2" || file_type == "P5")
    {
        int maxShade;
        image1_ifile >> maxShade;
        image2_ifile >> maxShade;

        //std::vector<std::vector<int>> matrix1 = deserialize_matrix(image1);
        //std::vector<std::vector<int>> matrix2 = deserialize_matrix(image2);

        std::vector<std::vector<int>> matrix1(height);
        std::vector<std::vector<int>> matrix2(height);
        int element;
        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image1_ifile >> element;
                matrix1[i].push_back(element);
            }
        }

        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image2_ifile >> element;
                matrix2[i].push_back(element);
            }
        }
        image1_ifile.close();
        image2_ifile.close();

        std::vector<std::vector<int>> outmatrix(height);

        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width * 2; ++j) // horizontal
            {
                if(j < width)
                {
                    outmatrix[i].push_back(matrix1[i][j]); 

                }else if(j >= width){
                    outmatrix[i].push_back(matrix2[i][j - width]);
                }
            }
        }

        serialize_matrix_with_max_value(outimage, outmatrix, file_type, width * 2, height, maxShade);
    }
    else if(file_type == "P3" || file_type == "P6")
    {
        int maxColour;
        image1_ifile >> maxColour;
        image2_ifile >> maxColour;

        //std::vector<std::vector<Pixel>> matrix1 = deserialize_pixel_matrix(image1);
        //std::vector<std::vector<Pixel>> matrix2 = deserialize_pixel_matrix(image2);

        std::vector<std::vector<Pixel>> matrix1(height);
        std::vector<std::vector<Pixel>> matrix2(height);
        Pixel pixel;
        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image1_ifile >> pixel;
                matrix1[i].push_back(pixel);
            }
        }

        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image2_ifile >> pixel;
                matrix2[i].push_back(pixel);
            }
        }
        image1_ifile.close();
        image2_ifile.close();

        std::vector<std::vector<Pixel>> outmatrix(height);

        for(size_t i = 0; i < height; ++i)
        {
            for(size_t j = 0; j < width * 2; ++j) // horizontal
            {
                if(j < width)
                {
                    outmatrix[i].push_back(matrix1[i][j]); 

                }else if(j >= width){
                    outmatrix[i].push_back(matrix2[i][j - width]);
                }
            }
        }

        serialize_pixel_matrix_with_max_value(outimage, outmatrix, file_type, width * 2, height, maxColour);
    }
    else
    {
        std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
    }
}

void Session::collage_vertical(const std::string& image1, const std::string& image2, const std::string& outimage)
{
    std::ifstream image1_ifile(image1);
    std::ifstream image2_ifile(image2);

    std::string file_type;
    size_t width, height; 

    image1_ifile >> file_type;
    image1_ifile >> width >> height;

    image2_ifile >> file_type;
    image2_ifile >> width >> height;

    if(file_type == "P1" || file_type == "P4")
    {
        //std::vector<std::vector<int>> matrix1 = deserialize_matrix(image1);
        //std::vector<std::vector<int>> matrix2 = deserialize_matrix(image2);

        std::vector<std::vector<int>> matrix1(height);
        std::vector<std::vector<int>> matrix2(height);
        int element;
        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image1_ifile >> element;
                matrix1[i].push_back(element);
            }
        }

        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image2_ifile >> element;
                matrix2[i].push_back(element);
            }
        }
        image1_ifile.close();
        image2_ifile.close();

        std::vector<std::vector<int>> outmatrix(height * 2);

        for(size_t i = 0; i < height * 2; ++i)
        {
            if(i < height){

                for(size_t j = 0; j < width; ++j) // vertical
                {
                    outmatrix[i].push_back(matrix1[i][j]);
                }
            }
            else if(i >= height)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    outmatrix[i].push_back(matrix1[i - height][j]);
                }
            }
        }

        serialize_matrix(outimage, outmatrix, file_type, width, height * 2);
    }
    else if(file_type == "P2" || file_type == "P5")
    {
        int maxShade;
        image1_ifile >> maxShade;
        image2_ifile >> maxShade;

        //std::vector<std::vector<int>> matrix1 = deserialize_matrix(image1);
        //std::vector<std::vector<int>> matrix2 = deserialize_matrix(image2);

        std::vector<std::vector<int>> matrix1(height);
        std::vector<std::vector<int>> matrix2(height);
        int element;
        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image1_ifile >> element;
                matrix1[i].push_back(element);
            }
        }

        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image2_ifile >> element;
                matrix2[i].push_back(element);
            }
        }
        image1_ifile.close();
        image2_ifile.close();

        std::vector<std::vector<int>> outmatrix(height * 2);

        for(size_t i = 0; i < height * 2; ++i)
        {
            if(i < height){

                for(size_t j = 0; j < width; ++j) // vertical
                {
                    outmatrix[i].push_back(matrix1[i][j]);
                }
            }
            else if(i >= height)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    outmatrix[i].push_back(matrix1[i - height][j]);
                }
            }
        }

        serialize_matrix_with_max_value(outimage, outmatrix, file_type, width, height * 2, maxShade);
    }
    else if(file_type == "P3" || file_type == "P6")
    {
        int maxColour;
        image1_ifile >> maxColour;
        image2_ifile >> maxColour;

        //std::vector<std::vector<Pixel>> matrix1 = deserialize_pixel_matrix(image1);
        //std::vector<std::vector<Pixel>> matrix2 = deserialize_pixel_matrix(image2);

        std::vector<std::vector<Pixel>> matrix1(height);
        std::vector<std::vector<Pixel>> matrix2(height);
        Pixel pixel;
        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image1_ifile >> pixel;
                matrix1[i].push_back(pixel);
            }
        }

        for(size_t i = 0; i < height; ++i){
            for(size_t j = 0; j < width; ++j){
                image2_ifile >> pixel;
                matrix2[i].push_back(pixel);
            }
        }
        image1_ifile.close();
        image2_ifile.close();

        std::vector<std::vector<Pixel>> outmatrix(height * 2);

        for(size_t i = 0; i < height * 2; ++i)
        {
            if(i < height){
                
                for(size_t j = 0; j < width; ++j) // vertical
                {
                    outmatrix[i].push_back(matrix1[i][j]);
                }
            }
            else if(i >= height)
            {
                for(size_t j = 0; j < width; ++j)
                {
                    outmatrix[i].push_back(matrix1[i - height][j]);
                }
            }
        }

        serialize_pixel_matrix_with_max_value(outimage, outmatrix, file_type, width, height * 2, maxColour);
    }
    else
    {
        std::cout << "The program only works with PBM, PGM, PPM files." << std::endl;
    }
}

void Session::collage(const std::string& direction, const std::string& image1, 
                             const std::string& image2, const std::string& outimage)
{
    if(direction != "horizontal" && direction != "vertical")
    {
        throw std::invalid_argument("Wrong direction entered. Neither 'horizontal', nor 'vertical'.");
    }

    if(foundImage(image1) != 1 || foundImage(image2) != 1)
    {
        throw image1 + " and/or " + image2 + " not found.";
    }

    if(sameType(image1, image2) != 1)
    {
        throw "Cannot make a collage from different types! (" + getType(image1) + " and " + getType(image2) + ")\'n'"; 
    }

    if(fileSize(image1) != fileSize(image2))
    {
        throw "Different sizes.";
    }
        
    if(direction == "horizontal")
    {
        collage_horizontal(image1, image2, outimage);
    }
    else if(direction == "vertical")
    {
        collage_vertical(image1, image2, outimage);
    }

    std::cout << "New collage '" << outimage << "' created." << std::endl;
    addImage(outimage);
}