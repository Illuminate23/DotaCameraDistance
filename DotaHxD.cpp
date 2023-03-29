#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

class Exception
{
private:
    std::string msg_;
public:
    Exception(const std::string & msg) : msg_(msg) {}
    ~Exception() {}

    std::string getMessage() const { return(msg_); }
};


class DotaCameraDistance
{
private:
    const unsigned char search_value[4] = { 0x00, 0x00, 0x96, 0x44 };
    std::fstream file;
public:
    DotaCameraDistance()
    {
        file.open("C:\\Program Files (x86)\\Steam\\steamapps\\common\\dota 2 beta\\game\\dota\\bin\\win64\\client.dll",
            std::ios::in | std::ios::out | std::ios::binary);
        if (!file)
        {
            throw(Exception("File doesn`t exist!"));
        }
    }
    ~DotaCameraDistance()
    {
        file.close();
    }
    int GetCameraDistance() const
    {
        std::cout << "Enter camera distance: ";
        int distance;
        std::cin >> distance;
        return distance;
    }
    unsigned char* ConvertDistance(float Distance) const
    {
        unsigned char* converted_distance = reinterpret_cast<unsigned char*>(&Distance);

        return converted_distance;

    }
    void ChangeCameraDistance() 
    {
        unsigned char* replace_value = ConvertDistance(GetCameraDistance());
        
        file.seekg(0, std::ios::end);
        int file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        char* buffer = new char[file_size];
        file.read(buffer, file_size);
        
        char* p = buffer;
        int num_replacements = 0;
        while (p < buffer + file_size) {
            if (memcmp(p, search_value, sizeof(search_value)) == 0) {
                memcpy(p, replace_value, sizeof(replace_value));
                num_replacements++;
            }
            p++;
        }

        if (num_replacements > 0) {
            std::cout << "Value founded and changed" << num_replacements << " times." << std::endl;
            file.seekp(0, std::ios::beg);
            file.write(buffer, file_size);
        }
        else {
            throw(Exception("Value doesn`t founded or it`s already changed!"));
        }
    }
};

int main() {

    try
    {
        DotaCameraDistance distance;
        distance.ChangeCameraDistance();
        system("pause");
        return EXIT_SUCCESS;
    }
    catch (Exception& e)
    {
        std::cout << "Error: " << e.getMessage() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
}
