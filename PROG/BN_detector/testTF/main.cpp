#include <iostream>
#include "../tf.hpp"
#include "../Matrix.hpp"

int main() {
    // https://anyconv.com/fr/convertisseur-de-png-en-ppm/
    TF myTF;
    ste::Matrix<double> img = myTF.ToProcessed(myTF.ReadPPM("test"), 0.2);
    myTF.PrintPPM(img, "cam");
    return 0;
}
