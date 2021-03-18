#ifndef FILTER_H
#define FILTER_H

#include <string>

class Filter {
public:
    Filter();
    double getWidth();
    std::string& getBorder();
    std::string& getType();
    double getValue(double value);
    void setWidth(double w);
    void setBorder(std::string& b);
    void setType(std::string& newType);

private:
    std::string type;
    std::string border;
    double width;
    double sinc(double x);
};


#endif //FILTER_H
