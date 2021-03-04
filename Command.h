#ifndef COMMAND_H
#define COMMAND_H

#include <string>

/*
 * command abstract class, represents all possible commands
 */
class Command {
public:
    /*
     * method: execute
     * details: performs the job of the command
     */
    virtual void execute();

    /* method: toString
     * return: string representation of this command
    */
    virtual std::string toString() const;

    virtual ~Command() {};
};

#endif // COMMAND_H
