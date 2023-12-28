#include "Config.hpp"
#include "Request.hpp"
#include <dirent.h>

std::string  LandingPage(std::string path)
{

    DIR *dir = opendir(path.c_str()); // DIR is struct handle directory stream //  and opendir return pointer to the directory
    if (dir == NULL)         // check if null // occur when not found any directory of this path
    {
        perror("failed to open dir");
    }

    struct dirent *entry;
    std::string response = "<html>\n\
    <head>\n\
    <title>AutoIndex</title>\n\
    <style>\n\
    body{\n\
        background-color:gray;\n\
    }\n\
    .links{\n\
        display: grid;  grid-template-columns: repeat(3, 1fr);\n\
        gap:80px;\n\
        justify-content: space-between;\n\
        list-style-type: none;\n\
    }\n\
    .link{\n\
        padding: 20px;\n\
        list-style-type: none;\n\
        font-size: 24px;\n\
        font-weight: 500;\n\
        color:white;\n\
        text-decoration:none;\n\
    }\n\
    </style>\n\
    </head>\n\
    <body>\n\
    <ul class=\"links\">\n";
    std::stringstream ss;
    while ((entry = readdir(dir)) != NULL) // readdir func used to read contain the directory
    {

        if (entry->d_name[0] != '.')
        {
            ss << "<li  ><a class=\"link\" href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>\n";
        }
    }

    response.append(ss.str());
    response += "</body>\n\
    </html>";
    
    closedir(dir);
    return(response); //return html
}