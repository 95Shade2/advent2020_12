#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

int Int(string num) {
    int num_i = 0;

    while (num.size() > 0) {
        num_i *= 10;
        num_i += num[0] - '0';
        num = num.substr(1);
    }

    return num_i;
}

vector<string> File_To_Str_Vec(string filename) {
    vector<string> lines_int;
    string line;
    ifstream file;

    file.open(filename.c_str());

    getline(file, line);
    while (file) {
        lines_int.push_back(line);
        getline(file, line);
    }

    file.close();

    return lines_int;
}

void Rotate(double *x, double *y, int degrees) {
    bool right = true;
    double temp;

    //cout << "Rotating by " << degrees << endl;
    //cout << "\t(" << *x << ", " << *y << ")\n";

    if (degrees < 0) {
        right = false;
        degrees = abs(degrees);
    }

    while (degrees > 0) {
        //determine the pos/neg of each
        if (right) {
            //top-right
            if (*x > 0 && *y > 0) {
                *x *= -1; //goes to the bottom-right
            }
            //bottom-right
            else if (*x > 0) {
                *x *= -1; //goes to bottem-left
            }
            //bottom-left
            else if (*y < 0) {
                *x *= -1;   //goes to the top-left
            }
            //top-left
            else {
                *x *= -1;   //goes to top-right
            }
        }
        else {
            //top-right
            if (*x > 0 && *y > 0) {
                *y *= -1; //goes to the top-left
            }
            //bottom-right
            else if (*x > 0) {
                *y *= -1; //goes to top-right
            }
            //bottom-left
            else if (*y < 0) {
                *y *= -1;   //goes to the bottom-right
            }
            //top-left
            else {
                *y *= -1;   //goes to bottom-left
            }
        }

        //swap
        temp = *x;
        *x = *y;
        *y = temp;

        //cout << "\t(" << *x << ", " << *y << ")\n";

        degrees -= 90;
    }

    //cout << endl;
}

void Run_Instruction(string instruction, double *west_east, double *north_south, int *facing, double *way_x = 0, double *way_y = 0, int part = 1) {
    char command = instruction[0];
    int dist = Int(instruction.substr(1));

    switch(command) {
    case 'F':
        if (part == 1) {
            switch(*facing) {
            case 0:
                (*north_south) += dist; //north
                break;
            case 90:
                (*west_east) += dist;   //east
                break;
            case 180:
                (*north_south) -= dist; //south
                break;
            case 270:
                (*west_east) -= dist;   //west
                break;
            }
        }
        else {
            (*north_south) += ((*way_y) * dist);
            (*west_east) += ((*way_x) * dist);
        }
        break;
    case 'N':
        if (part == 1) {
            (*north_south) += dist;
        }
        else {
            (*way_y) += dist;
        }
        break;
    case 'S':
        if (part == 1) {
            (*north_south) -= dist;
        }
        else {
            (*way_y) -= dist;
        }
        break;
    case 'E':
        if (part == 1) {
            (*west_east) += dist;
        }
        else {
            (*way_x) += dist;
        }
        break;
    case 'W':
        if (part == 1) {
            (*west_east) -= dist;
        }
        else {
            (*way_x) -= dist;
        }
        break;
    case 'R':
        if (part == 1) {
            (*facing) += dist;
            (*facing) %= 360;
        }
        else {
            Rotate(way_x, way_y, dist);
        }
        break;
    case 'L':
        if (part == 1) {
            (*facing) -= dist;
            if ((*facing) < 0) (*facing) = 360 + (*facing);
        }
        else {
            Rotate(way_x, way_y, (dist*-1));
        }
        break;
    default:
        cout << "No case for " << command << endl;
    }
}

double Get_Man_Dist(vector<string> instructions, int part = 1) {
    double man_dist = 0;
    double west_east = 0;  //neg means west, pos means east
    double north_south = 0;    //neg means south, pos means north
    int facing = 90; //0 = north, 90 = east, 180 = south, 270 = west
    double way_x = 10;
    double way_y = 1;

    for (int ip = 0; ip < instructions.size(); ip++) {
        if (part == 1) {
            Run_Instruction(instructions[ip], &west_east, &north_south, &facing);
        }
        else {
            Run_Instruction(instructions[ip], &west_east, &north_south, &facing, &way_x, &way_y, 2);
        }
    }

    cout << west_east << "\t" << north_south << endl;

    man_dist = abs(west_east) + abs(north_south);

    return man_dist;
}

int main()
{
    vector<string> instructions;
    double man_dist;

    instructions = File_To_Str_Vec("instructions.txt");

    man_dist = Get_Man_Dist(instructions);

    cout << man_dist << endl;

    //part 2

    man_dist = Get_Man_Dist(instructions, 2);

    cout << man_dist << endl;

    return 0;
}
