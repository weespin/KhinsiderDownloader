
#ifndef MAIN_H
#define MAIN_H
#include "maincontroller.h"
//TODO REMOVE IT!
class AppSingleton {
public:
    static AppSingleton& instance() {
        static AppSingleton _instance;
        return _instance;
    }
    void SetAppController( AppController* controller) {
        m_pAppController = controller;
    }
    AppController* GetAppController() {return m_pAppController;}



private:
    AppSingleton() {

    }

    ~AppSingleton() {

    }

    AppController* m_pAppController;
    Q_DISABLE_COPY(AppSingleton);
};
#endif //MAIN_H
