#include <stdio.h>

int main() {
    double length, width, area;

    printf("Enter the width of the rectangle: ");
    scanf("%lf", &width);
    
    printf("Enter the length of the rectangle: ");
    scanf("%lf", &length);

    area = length * width;

    printf("The area of the rectangle is: %.2lf square units\n", area);

    return 0;
}

