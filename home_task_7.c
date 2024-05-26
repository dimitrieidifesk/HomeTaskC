#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>

void printArray(char* array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", array[i]);
    }
    printf("\n");
}
    

int main() {
    setlocale(LC_ALL, "Rus");
    FILE* inputFile, * outputFile;
    char character;

    inputFile = fopen("input_c_code.txt", "r");

    outputFile = fopen("output_c_code.txt", "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Ошибка при открытии файлов.\n");
        return 1;
    }
    int parenthesis = 0;
    int brace = 0;
    int skip_space = 0;
    char* textArray = NULL;
    int arraySize = 0;

    while ((character = fgetc(inputFile)) != EOF) {
        textArray = (char*)realloc(textArray, (arraySize + 1) * sizeof(char));
        textArray[arraySize++] = character;
    }
    textArray = (char*)realloc(textArray, (arraySize + 1) * sizeof(char)); // адаптируем размер массива
    textArray[arraySize] = ' ';
    printArray(textArray, arraySize);

    char* newTextArray = NULL;
    int j = 0; 
    for (int i = 0; i < arraySize - 1; i++) {
        if (textArray[i] == '{' || textArray[i] == '}') {
            int k = i;
            if (newTextArray[j - 1] != "}" || newTextArray[j - 1] != "{") {
                while (textArray[k - 1] == ' ' && k > 0) {
                    k--;
                }
            }
            
            newTextArray = (char*)realloc(newTextArray, (j + (k - i) + 1) * sizeof(char));
            newTextArray[j+(k-i)] = textArray[i];
            j = j + (k - i) + 1;
        }
        else {

            newTextArray = (char*)realloc(newTextArray, (j+1) * sizeof(char));
            newTextArray[j++] = textArray[i];
        }
    }

    for (int i = 0; i < j; i++) {
        character = newTextArray[i];
        // Если символ - открывающая фигурная скобка, делаем отступ
        if (character == '{') {
            brace += 1;
            fputc('{', outputFile);
            fputc('\n', outputFile);
            for (int i = 0; i < 4 * brace; i++) {
                fputc(' ', outputFile);
            }
            skip_space = 1;
        }
        // Если символ - закрывающая фигурная скобка, переносим строку
        else if (character == '}') {

            fputc('\n', outputFile);
            for (int i = 0; i < 4 * (brace - 1); i++) {
                fputc(' ', outputFile);
            }
            fputc('}', outputFile);
            
            if (newTextArray[i + 1] != '}') {
                fputc('\n', outputFile);
                for (int i = 0; i < 4 * (brace - 1); i++) {
                    fputc(' ', outputFile);
                }
            }
            brace -= 1;
            skip_space = 1;
        }
        else if (character == '(') {
            parenthesis += 1;
            fputc(character, outputFile);
        }
        else if (character == ')') {
            parenthesis -= 1;
            fputc(character, outputFile);
        }
        else if (character == ';') {
            fputc(';', outputFile);
            if (parenthesis == 0) {
                
                if (newTextArray[i + 1] != '}') {
                    fputc('\n', outputFile);
                    for (int i = 0; i < 4 * (brace); i++) {
                        fputc(' ', outputFile);
                    }
                }
                skip_space = 1;
            }
        }
        else if (character == '\n') {
            fputc(character, outputFile);
            for (int i = 0; i < 4 * (brace); i++) {
                fputc(' ', outputFile);
            }
            
        }
        else {
            if (character == ' ' && skip_space == 1)
                continue;
            else if (skip_space == 1)
                skip_space = 0;
            fputc(character, outputFile);
        }

    }


    fclose(inputFile);
    fclose(outputFile);
    free(textArray);
    free(newTextArray);

    printf("Программа успешно отредактирована.\n");

    return 0;
}

