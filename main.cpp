#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <iterator>

struct Object {
    int numObject;
    int left;
    int right;
    double middle;
    int length;
    bool isRestrictedArea;
};

struct Line {
    int length;
    int numOfRestrictedAreas;
    std::list<Object> *objectsInLine;
};

Line *createLine(char *fileName) {
    Line *line1 = new Line();
    std::ifstream fin(fileName);
    fin >> line1->length;
    fin >> line1->numOfRestrictedAreas;
    line1->objectsInLine = new std::list<Object>();
    for (int i = 0; i < line1->numOfRestrictedAreas; i++) {
        Object *restr = new Object();
        restr->isRestrictedArea = true;
        fin >> restr->numObject;
        fin >> restr->left;
        fin >> restr->right;
        restr->length = restr->right - restr->left;
        restr->middle = restr->left + restr->length / 2.;
        line1->objectsInLine->push_back(*restr);
        delete restr;
    }
    fin.close();
    return line1;
}

void outputLine(Line line) {
    std::cout << line.length << " " << line.numOfRestrictedAreas << std::endl;
    std::list<Object>::iterator it = line.objectsInLine->begin();
    int i = 0;
    while(it!=line.objectsInLine->end()) {
        std::cout << "Area" << i << ":" << std::endl;
        std::cout << it->numObject << " " << it->isRestrictedArea << " " << it->length << " " << it->left << " "
                  << it->right << std::endl;
        advance(it, 1);
        i++;
    }
}

void deleteLine(Line line) {
    line.objectsInLine->clear();
}

Object *createObjects(char *fileName, int *num) {
    std::ifstream fin(fileName);
    fin >> *num;
    Object *objects = new Object[*num];
    for (int i = 0; i < *num; i++) {
        fin >> objects[i].numObject;
        fin >> objects[i].length;
        objects[i].isRestrictedArea = false;
    }
    return objects;
}

void sortBySize(Object *objects, int size) {
    Object tmp;
    for (int i = 0; i < size; i++) {
        for (int j = (size - 1); j >= (i + 1); j--) {
            if (objects[j].length < objects[j - 1].length) {
                tmp = objects[j];
                objects[j] = objects[j - 1];
                objects[j - 1] = tmp;
            }
        }
    }
}

void printObjects(Object *objects, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << objects[i].length << " " << objects[i].numObject << std::endl;
    }
}

bool addObject(Line line, Object object) {
    std::list<Object>::iterator it = line.objectsInLine->begin();
    std::list<Object>::iterator itRight = it;
    bool isFit = false;
    if (it->left >= object.length) {
        object.left = 0;
        object.right = object.left + object.length;
        object.middle = object.length / 2.;
        line.objectsInLine->push_front(object);
        isFit = true;
    } else {
        advance(itRight, 1);
    }
    while ((itRight != line.objectsInLine->end()) && !isFit) {
        if (itRight->left - it->right >= object.length) {
            object.left = it->right;
            object.right = object.left + object.length;
            object.middle = object.left + object.length / 2.;
            line.objectsInLine->insert(itRight, object);
            isFit = true;
        } else {
            advance(it, 1);
            advance(itRight, 1);
        }
    }
    if (!isFit) {
        if (line.length - it->right >= object.length) {
            object.left = it->right;
            object.right = object.left + object.length;
            object.middle = object.left + object.length / 2.;
            line.objectsInLine->push_back(object);
            isFit = true;
        }
    }
    return isFit;
}

void addObjects(Line line1, Line line2, Object *objects, int objectsSize) {
    for (int i = objectsSize - 1; i >= 0; i--) {
        if (!addObject(line1, objects[i])) {
            addObject(line2, objects[i]);
        }
    }
}

int** readMatrix(char* fileName, int *height, int *width) {
    std::ifstream fin(fileName);
    fin >> *height >> *width;
    int** matrix = new int*[*height];
    for(int i = 0; i < *height; i++) {
        matrix[i] = new int[*width];
    }
    for(int i = 0; i < *height; i++)
        for(int j = 0; j < *width; j++)
            fin >> matrix[i][j];
    return matrix;
}

void deleteMatrix(int** matrix, int height, int width) {
    for(int i = 0; i < height; i++)
        delete[] matrix[i];
    delete [] matrix;
}

void printMatrix (int** matrix, int height, int width) {
    for(int i = 0; i < height; i++){
        for(int j =0; j < width; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
}



double countBindCostInLines(Line line1, Line line2) {

}

int main() {
    std::ofstream fout("Output");
    Line *line1 = createLine("InputLine1");
    Line *line2 = createLine("InputLine2");
    int size;
    int matrBindCostHeight, matrBindCostWidth;
    int matrRestrictedCostHeight, matrRestrictedCostWidth;
    int** matrixBindCost = readMatrix("Matrix", &matrBindCostHeight, &matrBindCostWidth);
    int** matrixRestrictedCosts = readMatrix("MatrixRestricted", &matrRestrictedCostHeight, &matrRestrictedCostWidth);
    printMatrix(matrixBindCost, matrBindCostHeight, matrBindCostWidth);
    printMatrix(matrixRestrictedCosts, matrRestrictedCostHeight, matrRestrictedCostWidth);
    Object *objects = createObjects("InputObjects", &size);
    sortBySize(objects, size);
    printObjects(objects, size);
    addObjects(*line1, *line2, objects, size);
    outputLine(*line1);
    outputLine(*line2);
    deleteLine(*line2);
    deleteMatrix(matrixBindCost, matrBindCostHeight, matrBindCostWidth);
    deleteMatrix(matrixRestrictedCosts, matrRestrictedCostHeight, matrRestrictedCostWidth);
    deleteLine(*line1);
    delete[]objects;
    delete line2;
    delete line1;
    return 0;
}