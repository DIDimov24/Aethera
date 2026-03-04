#pragma once

#include <QString>

struct Question {
    QString text;
    QString optionA;
    QString optionB;
    QString optionC;
    QString optionD;
    int correctAnswer;
    QString category;
};
