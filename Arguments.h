#ifndef ARGUMENTS_H_INCLUDED
#define ARGUMENTS_H_INCLUDED

#include "DataFunction.h"
#include "function_traits.h"
#include <tuple>
#include "LTestMisuseException.h"

using namespace std;


template<typename... Types>
class Arguments{
    DataFunctionBase* dataFunction;
    tuple<Types...> storedArgs;
    bool dataFuncReturnsVoid;
    bool executed;

    void checkAndThrowWhenVoidFunction(string&& errorMessage){
    	if(dataFuncReturnsVoid){
    		throw ExpectAtVoid(errorMessage);
    	}
    }

public:

    Arguments(DataFunctionBase* datafunc, Types... args):dataFunction(datafunc), storedArgs(args...) {
        dataFuncReturnsVoid = datafunc->isVoidReturn();
        if(dataFuncReturnsVoid){
            executed = true;
            (dynamic_cast<DataFunction<void, Types...>*>(dataFunction))->run(args...);
        }else{
            executed = false;
        }
    }

    ~Arguments(){
        if(!executed){
            throw MissingExpect("fixture is not executed. use expect()");
        }
    }

    template<typename Ret>
    void expect(Ret expectedValue){
    	checkAndThrowWhenVoidFunction("void function can't expect anything");
        executed = true;
        (dynamic_cast<DataFunction<Ret, Types...>*>(dataFunction))->runWithTuple(expectedValue, storedArgs);
    }

    template<typename Funct>
    void validate(Funct validator){
        typedef typename GetFunctParamType<Funct>::type Ret;
        checkAndThrowWhenVoidFunction("void function can't validate anything");
        executed = true;
        (dynamic_cast<DataFunction<Ret, Types...>*>(dataFunction))->validate(validator, storedArgs);
    }

    template<typename T>
    void anything(){
    	checkAndThrowWhenVoidFunction("void function get nothing not anything");
        executed = true;
        (dynamic_cast<DataFunction<T, Types...>*>(dataFunction))->validate([](T t){return true;}, storedArgs);
    }
};

#endif // ARGUMENTS_H_INCLUDED
