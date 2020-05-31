#include "commands.h"

void Commands::exit()
{
    std::cout << "Exiting the program..." << std::endl;
}

void Commands::help()
{
    std::cout << "The following commands are supported:" << std::endl;
    std::cout << std::endl;
    std::cout << "open <file>   -> opens <file>" << std::endl;
    std::cout << "close         -> closes currently opened file / saves the current session" << std::endl;
    std::cout << "save          -> applies the commands on the images in the current session" << std::endl;
    std::cout << "saveas <file> -> applies the commands on the images in the current session and saves the first image in <file>" << std::endl;
    std::cout << "help          -> prints this information" << std::endl;
    std::cout << "exit          -> exists the program" << std::endl;
    std::cout << std::endl;
    std::cout << "grayscale                 -> turns an image into gray-shaded image" << std::endl;
    std::cout << "monochrome                -> turns an image into a black & white image" << std::endl;
    std::cout << "negative                  -> the colours in an image are reversed into their respective complementary colours" << std::endl;
    std::cout << "rotate <direction>        -> rotates an image 90 degrees LEFT or RIGHT" << std::endl;
    std::cout << "undo                      -> removes the last transformation made in the current session" << std::endl;
    std::cout << "add <image>               -> adds an image to the current session" << std::endl;
    std::cout << "session info              -> prints all the information up to this moment about the current session" << std::endl;
    std::cout << "switch <session>          -> switches to a session with identification number <session>" << std::endl;
    std::cout << std::endl;
    std::cout << "collage <direction>       -> horizontal or vertical" << std::endl;
    std::cout << "         <image1> <image2> -> makes a collage of these two images" << std::endl;
    std::cout << "          <outimage>        -> the result is saved in <outimage>" << std::endl;
}

bool Commands::foundID(int _ID){
    bool found = false;

    std::ifstream ifile("sessions.txt");

    if(ifile.good()){
        std::string line;
        while(std::getline(ifile, line))
        {
            if(stoi(line) == _ID){          // stoi() takes a string as an argument and returns its value
                found = true;
                break;
            }
        }
    }else{
        std::cout << "File couldn't be opened." << std::endl;
    }
    ifile.close();

    return found;
}

Session Commands::load(const std::string& image)
{
    std::fstream myfile("sessions.txt", std::ios::out | std::ios::in | std::ios::app);
    myfile.seekg(0, std::ios::end);
    int file_size = myfile.tellg();
    myfile.close();

    Session session;
    session.changeID(file_size);
    
    int j = 0;
    std::string word;
    std::cout << std::endl;
    std::cout << "Session with ID: " << file_size << " started." << std::endl;

    for(int i = 0; i < image.size(); ++i)
    {
        if(image[i] == ' ' || i == image.size() - 1)
        {
            word = image.substr(j, i - j);
            if(i == image.size() - 1)
            {
                word = image.substr(j, i - j + 1);
            }
            if(correctImage(word) == 1){
                session.add(word);
            }else{
                std::cout << "Could not add this image, wrong type." << std::endl;
            }
            j = i + 1;
            ++i;
        }
    }

    return session;
}

void Commands::serialization(const Session& session)
{
    std::ofstream ofile("sessions.txt", std::ios::app);

    if(ofile.good()){
        ofile << session.getID() << std::endl;

        ofile << session.getImagesSize() << " ";
        for(size_t i = 0; i < session.getImagesSize(); ++i)
        {
            ofile << session.getImage(i) << " ";
        }
        ofile << std::endl;

        ofile << session.getCommandsSize() << " ";
        for(size_t i = 0; i < session.getCommandsSize(); ++i)
        {
            ofile << session.getCommand(i) << " ";
        }
        ofile << std::endl;

    }else{
        std::cout << "File couldn't be opened." << std::endl;
    }
    ofile.close();
}

Session Commands::deserialization(int _ID)
{
    Session result;
    std::ifstream ifile("sessions.txt");

    if(ifile.good()){
        std::string line;
        while(std::getline(ifile, line))
        {
            if(stoi(line) == _ID){           // stoi() takes a string as an argument and returns its value
                result.changeID(_ID);

                size_t size_images;
                ifile >> size_images;
                for(size_t i = 0; i < size_images; ++i)
                {
                    std::string image;
                    ifile >> image;
                    result.addImage(image);
                }

                size_t size_commands;
                ifile >> size_commands;
                for(size_t i = 0; i < size_commands; ++i)
                {
                    std::string command;
                    ifile >> command;
                    result.addCommand(command);
                }
                break;
            }
        }
    }else{
        std::cout << "File couldn't be opened." << std::endl;
    }
    ifile.close();

    return result;
}

Session Commands::_switch(int _ID)
{
    Session current;

    if(foundID(_ID) == 1){
        current = deserialization(_ID);

        std::cout << "You switched to session with ID: " << current.getID() << std::endl;
        std::cout << "Name of images in the session: ";
        for(size_t i = 0; i < current.getImagesSize(); ++i)
        {
            std::cout << current.getImage(i) << " ";
        }
        std::cout << std::endl;
        std::cout << "Pending transformations: ";
        for(size_t i = 0; i < current.getCommandsSize(); ++i)
        {
            std::cout << current.getCommand(i) << " ";
        }
        std::cout << std::endl;

    }else{
        std::cout << "Session with ID '" << _ID << "' couldn't be found." << std::endl;
    }

    return current;
}

Session Commands::open(const std::string& file_name)
{
    Session result;

    if(file_name == "sessions.txt"){
        std::cout << "Successfully opened '" << file_name << "'." << std::endl;

        int ID;
        std::cout << "Enter the ID of the session you want to open." << std::endl;
        std::cin >> ID;

        if(foundID(ID) == 1){
            result = deserialization(ID);
        }else{
            std::cout << "Invalid ID." << std::endl;
        }
        
    }else{
        std::ofstream ofile(file_name, std::ios::app);
        ofile.close();
        std::cout << "File '" << file_name << "' has been created but it's empty." << std::endl;
    }

    return result;
}

void Commands::close(const std::string& file_name, const Session& session)
{
    if(file_name == "sessions.txt"){
        serialization(session);
        std::cout << "Successfully closed '" << file_name << "'." << std::endl;
    }else{
        std::cout << "File '" << file_name << "' doesn't meet the requirements for this program." << std::endl; 
        std::cout << "Unsuccessfully closed." << std::endl; 
    }
}

void Commands::save(Session& session)
{
    for(size_t i = 0; i < session.getCommandsSize(); ++i)
    {
        if(session.getCommand(i) == "grayscale"){
            session.grayscale();
        }else if(session.getCommand(i) == "monochrome"){
            session.monochrome();
        }else if(session.getCommand(i) == "negative"){
            session.negative();
        }else if(session.getCommand(i) == "rotate_right"){   
            session.rotate("right"); 
        }else if(session.getCommand(i) == "rotate_left"){
            session.rotate("left");
        }
    }      
}        

void Commands::saveas(Session& session, const std::string& new_file) // file for the first image of the current session
{
     for(size_t i = 0; i < session.getCommandsSize(); ++i)
    {
        if(session.getCommand(i) == "grayscale"){
            session.grayscale();
        }else if(session.getCommand(i) == "monochrome"){
            session.monochrome();
        }else if(session.getCommand(i) == "negative"){
            session.negative();
        }else if(session.getCommand(i) == "rotate_right"){   
            session.rotate("right"); 
        }else if(session.getCommand(i) == "rotate_left"){
            session.rotate("left");
        }
    }      
    
    // first image
    std::string line;
    std::ifstream ifile(session.getImage(0));
    std::ofstream ofile(new_file);
    while(std::getline(ifile, line)){
        ofile << new_file;
    }
    ifile.close();
    ofile.close();
}

void Commands::choices(const std::string& line, Session& session)
{
    if(line == "close"){            // saves current session
        std::string file_name;
        std::cout << "Write down the name of the file you want to close." << std::endl;
        std::getline(std::cin, file_name);
        close(file_name, session);

    }else if(line == "save"){
        serialization(session);     // saves current session
        save(session);

    }else if(line == "saveas"){
        std::string new_file;
        std::cout << "Write down a 'new_file' which you will save the first image of the current session in." << std::endl;
        std::getline(std::cin, new_file);

        serialization(session);     // saves current session
        saveas(session, new_file);

    }else if(line == "help"){
        help();

    }else if(line == "exit"){
        //serialization(session);   exit without saving current session
        exit();

    }else if(line == "grayscale"){
        if(session.foundCommand("grayscale") == 0){
            session.addCommand("grayscale");
        }

    }else if(line == "monochrome"){
        if(session.foundCommand("monochrome") == 0){
            session.addCommand("monochrome");
        }

    }else if(line == "negative"){
        // if(session.foundCommand("negative") == 0) { session.addCommand("negative"); }

        session.addCommand("negative");

    }else if(line == "rotate"){
        std::cout << "Enter a direction of rotation." << std::endl;
        std::string direction;
        std::getline(std::cin, direction);

        if(direction != "right" && direction != "left"){
            std::cout << "Wrong direction entered. Neither 'right', nor 'left'." << std::endl;
        }else{
            session.addCommand("rotate_" + direction);
        }
    
    }else if(line == "undo"){
        session.undo();

    }else if(line == "add"){
        std::string image;
        std::cout << "Write down the image you want to add to the current session." << std::endl;
        std::getline(std::cin, image);

        session.add(image);

    }else if(line == "session info"){
        session.session_info();       

    }else if(line == "collage"){

        try{    
            std::string direction;
            std::string image1;
            std::string image2;
            std::string outimage;
            
            std::cout << "Enter a direction for the collage." << std::endl;
            std::getline(std::cin, direction);
            std::cout << "Enter the name of the first image:" << std::endl;
            std::getline(std::cin, image1);
            std::cout << "Enter the name of the first image:" << std::endl;
            std::getline(std::cin, image2);
            std::cout << "Enter the name of the new image - collage of the last two." << std::endl;
            std::getline(std::cin, outimage);

            std::cout << std::endl;
            session.collage(direction, image1, image2, outimage);

        }catch (std::invalid_argument ex){

            std::cerr << "There was a problem with the direction for the collage: " << ex.what() << std::endl;

        }catch (std::string str){

            std::cerr << "There was a problem with the images used for the collage: " << str << std::endl;

        }

    }else{
        std::cout << "Invalid command." << std::endl;
    }
}