#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "commands.h"

// PBM monochrome (black & white)
// PGM grayscale   
// PPM full-colour

TEST_CASE("Correct Image")
{
    std::string image1 = "image1.pbm";
    CHECK(correctImage(image1) == 1);

    std::string image2 = "image2.pgm";
    CHECK(correctImage(image2) == 1);

    std::string image3 = "image3.ppm";
    CHECK(correctImage(image3) == 1);
}

TEST_CASE("Same type")
{
    std::string image1 = "image1.pbm";
    std::string image2 = "image2.pbm";
    CHECK(sameType(image1, image2) == 1);

    std::string image3 = "image3.pgm";
    std::string image4 = "image4.pgm";
    CHECK(sameType(image3, image4) == 1);

    std::string image5 = "image5.ppm";
    std::string image6 = "image6.ppm";
    CHECK(sameType(image5, image6) == 1);
}

TEST_CASE("Serialization and deserialization")
{
    Session session({"rotate_left", "negative", "monochrome"}, {"image1.ppm", "image2.pgm", "image3.pbm"}, 1);
    CHECK(session.getID() == 1);
    CHECK(session.getImage(2) == "image3.pbm");
    CHECK(session.getCommand(2) == "monochrome");

    //serialization(session);
    Session result = Commands::deserialization(session.getID());

    CHECK(result.getID() == 1);
    CHECK(result.getImage(2) == "image3.pbm");
    CHECK(result.getCommand(2) == "monochrome");
}

TEST_CASE("Testing commands")
{
    Session session({"rotate_left", "negative", "monochrome"}, {"image1.ppm", "image3.pbm"}, 7);
    session.undo(); 
    CHECK(session.getCommandsSize() == 2);
    CHECK(session.getCommand(1) == "negative");
    session.addCommand("grayscale");
    CHECK(session.getCommandsSize() == 3);
    CHECK(session.getCommand(1) == "negative");
    CHECK(session.getCommand(2) == "grayscale");
    session.addCommand("rotate_right");
    CHECK(session.getCommandsSize() == 4);
    CHECK(session.getCommand(3) == "rotate_right");
    session.addCommand("rotate_left");
    CHECK(session.getCommandsSize() == 5);
    CHECK(session.getCommand(4) == "rotate_left");
    session.add("image4.ppm");
    CHECK(session.getImagesSize() == 3);
    CHECK(session.getImage(2) == "image4.ppm");

    //serialization(session);
}

TEST_CASE("Testing File Sizes")
{
    std::string size1 = fileSize("P1.pbm"); // 24 7
    std::string size2 = fileSize("P2.pgm"); // 24 7 15(max value)
    std::string size3 = fileSize("P3.ppm"); //  4 4 15(max value)

    CHECK(size1 != size2);
    CHECK(size1 != size3);
}

TEST_CASE("Testing deserialize_matrix()")
{
    std::vector<std::vector<int>> matrix = deserialize_matrix("P1.pbm");

    CHECK(matrix[6][23] == 0);
}

TEST_CASE("Testing deserialize_pixel_matrix()")
{
    std::vector<std::vector<Pixel>> matrix = deserialize_pixel_matrix("P3.ppm");

    Pixel pixel1(0, 15, 7);
    Pixel pixel2(15, 0, 15);
    Pixel pixel3(14, 14, 14);
    CHECK((matrix[2][2] == pixel1) == 1);
    CHECK((matrix[3][0] == pixel2) == 1);
    CHECK((matrix[3][3] == pixel3) == 1);
}

int main()
{
    // g++ main.cpp session.cpp commands.cpp pixel.cpp -o main.exe

    // пускане на тестовете
    doctest::Context().run();

    std::cout << std::endl;
    std::cout << "If you know what you are doing, write down 'load' to start a new session and load at least one image." << std::endl;
    std::cout << "If you are not confident in your actions, write down 'help' to print information about the commands you can use." << std::endl;
    std::string choice;
    std:getline(std::cin, choice);

    if(choice == "load"){
        std::cout << "Write down the name/s of the images you want to load." << std::endl;
        std::string line;
        std::getline(std::cin, line);

        Session session = Commands::load(line);

        std::cout << std::endl;
        std::cout << "For more information about the commands write down 'help'." << std::endl;
        while(line != "exit"){
            std::cout << std::endl;
            std::cout << "Write down a command:" << std::endl;

            std::getline(std::cin, line);

            if(line == "switch"){
                //serialization(session);   // saves the information about the current session in sessions.txt

                int ID; 
                std::cout << "Enter the ID of the session which you want to switch to." << std::endl;
                std::cin >> ID;
                std::cin.ignore();

                session = Commands::_switch(ID); 

            }else{
                Commands::choices(line, session);
            }
        }
    }
    else if(choice == "help"){
        std::cout << std::endl;
        Commands::help();
        std::cout << std::endl;
        Commands::exit();
    }
    else{
        std::cout << "Wrong command entered." << std::endl;
        std::cout << std::endl;
        Commands::exit();
    }
    
    return 0;
}