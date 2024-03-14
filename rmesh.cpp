#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <filesystem>
namespace fs = std::filesystem;
std::vector<std::string> Split(const std::string &src, char tag)
{
    std::vector<std::string> out;
    size_t it = 0;
    size_t pos;
    while ((pos = src.find(tag, it)) != std::string::npos)
    {
        out.push_back(src.substr(it, pos-it));
        it = pos+1;
    }
    out.push_back(src.substr(it, src.size()-it));
    return out;
}
std::vector<std::vector<std::string>> ReadCSV(const std::string &path)
{
    std::vector<std::vector<std::string>> csv;
    std::ifstream f(path);
    std::string line;
    while (std::getline(f, line))
    {
        line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char x) { return std::isspace(x); }), line.end());
        csv.push_back(Split(line, ','));
    }
    f.close();
    return csv;
}
int GetIndex(std::vector<std::string> vec, std::string target)
{
    auto it = std::find(vec.begin(), vec.end(), target);
    if (it != vec.end()) 
    {
        return std::distance(vec.begin(), it);
    }
    return -1;
}
int main(int argc, char** argv)
{
    std::string path;
    std::string out_path;
    std::string posX;
    std::string posY;
    std::string posZ;
    std::string uvX;
    std::string uvY;
    std::string normalX;
    std::string normalY;
    std::string normalZ;
    if (argc == 6)
    {
        posX = argv[1];
        posY = argv[2];
        posZ = argv[3];
        path = argv[4];
        out_path = argv[5];
    }
    else if (argc == 8)
    {
        posX = argv[1];
        posY = argv[2];
        posZ = argv[3];
        uvX = argv[4];
        uvY = argv[5];
        path = argv[6];
        out_path = argv[7];
    }
    else if (argc == 9)
    {
        posX = argv[1];
        posY = argv[2];
        posZ = argv[3];
        normalX = argv[4];
        normalY = argv[5];
        normalZ = argv[6];
        path = argv[7];
        out_path = argv[8];
    }
    else if (argc == 11)
    {
        posX = argv[1];
        posY = argv[2];
        posZ = argv[3];
        uvX = argv[4];
        uvY = argv[5];
        normalX = argv[6];
        normalY = argv[7];
        normalZ = argv[8];
        path = argv[9];
        out_path = argv[10];
    }
    else
    {
        std::cout   << "input args error \n"
                    << "example \t\n"
                    << "\t 1. rmesh POSITION.x POSITION.y POSITION.z in_file.csv out_file.obj\n"
                    << "\t 2. rmesh POSITION.x POSITION.y POSITION.z UV.x UV.y in_file.csv out_file.obj\n"
                    << "\t 3. rmesh POSITION.x POSITION.y POSITION.z NORMAL.x NORMAL.y NORMAL.z in_file.csv out_file.obj\n"
                    << "\t 4. rmesh POSITION.x POSITION.y POSITION.z UV.x UV.y NORMAL.x NORMAL.y NORMAL.z in_file.csv out_file.obj" << std::endl;
        return -1;
    }
    std::vector<std::vector<std::string>> csv;
    std::ifstream ifs(path);
    if (ifs.is_open())
    {
        if (fs::path(out_path).parent_path().string() != "" && !fs::exists(fs::path(out_path).parent_path()))
        {
            fs::create_directories(fs::path(out_path).parent_path());
        }
        csv = ReadCSV(path);
        int pX = GetIndex(csv[0], posX);
        int pY = GetIndex(csv[0], posY);
        int pZ = GetIndex(csv[0], posZ);
        if (pX == -1 || pY == -1 || pZ == -1)
        {
            std::cout << "failed to find position " << posX << " " << posY << " " << posZ << std::endl;
            return -1;
        }
        int tX = GetIndex(csv[0], uvX);
        int tY = GetIndex(csv[0], uvY);
        if ((uvX != "" && uvY != "") && (tX == -1 || tY == -1))
        {
            std::cout << "failed to find uv " << uvX << " " << uvY << std::endl;
        }
        int nX = GetIndex(csv[0], normalX);
        int nY = GetIndex(csv[0], normalY);
        int nZ = GetIndex(csv[0], normalZ);
        if ((normalX != "" && normalY != "" && normalZ != "") && (nX == -1 || nY == -1 || nZ == -1))
        {
            std::cout << "failed to find normal " << normalX << " " << normalY << " " << normalZ << std::endl;
        }
        std::ofstream ofs(out_path);
        if (ofs.is_open() == false)
        {
            std::cout << "failed to open output file : " << out_path << std::endl;
            return -1;
        }
        for (int i = 1; i < csv.size(); i++)
        {
            ofs << "v " << csv[i][pX] << " " << csv[i][pY] << " " << csv[i][pZ] << std::endl;
        }
        if (tX != -1 && tY != -1)
        {
            for (int i = 1; i < csv.size(); i++)
            {
                ofs << "vt " << csv[i][tX] << " " << csv[i][tY] << std::endl;
            }
        }
        if (nX != -1 && nY != -1 && nZ != -1)
        {
            for (int i = 1; i < csv.size(); i++)
            {
                ofs << "vn " << csv[i][nX] << " " << csv[i][nY] << " " << csv[i][nZ] << std::endl;
            }
        }
        if (nX != -1 && nY != -1 && nZ != -1 && tX != -1 && tY != -1)
        {
            for (int i = 1; i < csv.size() - 2; i++)
            {
                ofs << "f " << i      << "/" << i     << "/" << i     << " "
                            << i + 1  << "/" << i + 1 << "/" << i + 1 << " "
                            << i + 2  << "/" << i + 2 << "/" << i + 2 << std::endl;
            }
        }
        else if (nX != -1 && nY != -1 && nZ != -1)
        {
            for (int i = 1; i < csv.size() - 2; i++)
            {
                ofs << "f " << i      << "//" << i     << " "
                            << i + 1  << "//" << i + 1 << " "
                            << i + 2  << "//" << i + 2 << std::endl;
            }
        }
        else if (tX != -1 && tY != -1)
        {
            for (int i = 1; i < csv.size() - 2; i++)
            {
                ofs << "f " << i      << "/" << i     << " "
                            << i + 1  << "/" << i + 1 << " "
                            << i + 2  << "/" << i + 2 << std::endl;
            }
        }
        else 
        {
            for (int i = 1; i < csv.size() - 2; i++)
            {
                ofs << "f " << i << " " << i + 1 << " " << i + 2 << std::endl;
            }
        }
        ofs.close();
        ifs.close();
        std::cout << "export file to " << out_path << " with position";
        if (nX != -1 && nY != -1 && nZ != -1)
        {
            std::cout << " normal" << std::endl;
        }
        if (tX != -1 && tY != -1)
        {
            std::cout << " uv" << std::endl;
        }
    }
    else
    {
        std::cout << "file open failed : " << path << std::endl;
        return -1;
    }
    return 0;
}
