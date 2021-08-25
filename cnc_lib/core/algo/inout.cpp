#include "inout.h"

cnc::cloud cnc::algo::parse_from_uchar(const std::string &filename,uint chunk_size,int max_size)
{
    std::ifstream file(filename);
    //file.open(filename,std::ios::in);
    if (!file)
        throw Cnc_error("Can't open given file " + filename);
    cnc::cloud R;

    cnc::vec buffer(chunk_size);
    char tmp;

    for (int k = 0;file;k++){
        if (max_size > 0 && k >= max_size)
            break;
        for (uint i = 0;i<chunk_size;i++){
            file.get(tmp);

            buffer(i) = (unsigned char)tmp;
        }
        R.add_point(buffer);
    }
    return R;
}

cnc::cloud cnc::algo::parse_csv(const std::string &filename)
{
    // Testing with regex if the file is a csv format
    std::regex e ("(.*)(\\.csv)");
    if (!std::regex_match(filename.begin(), filename.end(), e)){
        throw Cnc_error(filename + " is not a .csv file");
    }

    // Opening the file
    std::ifstream file(filename);
    if (!file){
        throw Cnc_error("Can't open the file: " + filename);
    }

    // Verifying the data before parsing and putting it in the cnc::cloud
    cnc::cloud R;
    std::string str;
    std::vector<std::string> tokens;
    getline(file, str);
    uint j = 0;
    while (getline(file, str)){
        tokens = split(str, ',');
        uint n = tokens.size();
        R.add_point(vec(n));
        for (uint k = 0;k<tokens.size();k++){
            try {
                if (tokens[k][0] == '.')
                    tokens[k] = std::string("0") + tokens[k];
                R[j](k) = std::stof(tokens[k]);
            }  catch (std::invalid_argument& e) {
                throw Cnc_error("couldn't convert " + tokens[k] + " to a number");
            }
        }
        j++;
    }
    return R;
}

std::vector<std::string> cnc::algo::split(const std::string &str, char delim,bool non_void)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delim)){
        if (non_void){
            if (token.size())
                tokens.push_back(token);
        }
        else {
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool cnc::algo::is_integer(const std::string &s)
{
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
    char * p;
    strtof(s.c_str(), &p);
    return (*p == 0);
}

void cnc::algo::export_cloud(const std::string &output_file, const cnc::cloud &C)
{
    std::ofstream file;
    file.open (output_file);
    file << C.size() << std::endl;
    for (const vec& p : C.points){
        for (uint k = 0;k<p.rowNum();k++)
            file << p(k) << ' ';
        file << std::endl;
    }
    file.close();
}

cnc::algo::geometry::SimpleGLMesh cnc::algo::import_mesh_from_obj(const std::string &input_file,float scale)
{
    std::ifstream file(input_file);
    if (!file.is_open())
        throw Cnc_error("Can't open the file: " + input_file);

    std::vector<QVector3D> vpos;
    std::vector<QVector3D> vnormals;
    std::vector<uint> faces;

    QVector3D tmp;
    std::string buffer;
    std::vector<std::string> tokens;
    bool quad_mesh = false;
    while (getline(file,buffer)){
        if (buffer.size() == 0)
            continue;
        tokens = split(buffer,' ',true);
        if (tokens[0] == "v"){
            for (uint k = 0;k<3;k++){
                std::stringstream ss(tokens[k+1]);
                ss >> tmp[k];
            }
            vpos.push_back(tmp*scale);
        } else if (tokens[0] == "vn"){
            for (uint k = 0;k<3;k++){
                std::stringstream ss(tokens[k+1]);
                ss >> tmp[k];
            }
            vnormals.push_back(tmp);
        } else if (tokens[0] == "f") {
            bool quad = tokens.size() == 6;
            std::vector<uint> ids;
            for (uint k = 0;k<3;k++){
                ids.push_back(std::stoi(split(tokens[k+1],'/')[0])-1);
                faces.push_back(ids.back());
            }
            if (quad){
                quad_mesh = true;
                ids.push_back(std::stoi(split(tokens[4],'/')[0])-1);
                faces.push_back(ids[2]);
                faces.push_back(ids[3]);
                faces.push_back(ids[0]);
            }
        }
    }
    if (quad_mesh)
        std::cout << "quad detected" << std::endl;
    return algo::geometry::SimpleGLMesh(vpos,vnormals,faces);
}
