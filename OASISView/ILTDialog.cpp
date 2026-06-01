#include <iostream>

#include "ILTDialog.h"

ILTDialog::ILTDialog(QWidget *parent)
{

}

ILTDialog::~ILTDialog() {

}

void ILTDialog::runButtonClicked(bool) {
    std::cout << "ILT RunButton" << std::endl;
}

void ILTDialog::cancelButtonClicked(bool) {
    std::cout << "ILT Cancel Button" << std::endl;
}
