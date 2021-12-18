//
// Created by Martin Gano on 18.12.2021.
//

#ifndef RAYTRACER_OBJPARSER_H
#define RAYTRACER_OBJPARSER_H


class ObjParser {
public:
    explicit ObjParser(char *obj_file_name);

private:
    char *obj_file_name_{};

};


#endif //RAYTRACER_OBJPARSER_H
