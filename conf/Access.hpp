#ifndef Access_HPP
# define Access_HPP
# include "Parser.hpp"
# include "Lexer.hpp"
# include <iostream>
class Access {
    private:
    Lexer *              _Lexer; //ici define inside the classs
    const std::vector<Parser *> * _Servers; // icidefine inside the class
    public:
    void start(void);
};

#endif