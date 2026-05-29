#ifndef CABALLOS_HPP
#define CABALLOS_HPP

#include <string>
#include <vector>
#include "common.h"

class Caballo {
    public:
        // inline function
        int getId() const { return id; }
        void setId(int id_) { id = id_; }

        const std::string& getName() const { return name; }
        void setName(const std::string& name_) { name = name_; }

        int getPosition() const { return position; }
        void setPosition(int position_) { position = position_; }
        int getVueltasActuales() const { return vueltas_actuales; }
        void setVueltasActuales(int v) { vueltas_actuales = v; }

        int getMetrosTotales() const { return metros_totales; }
        void setMetrosTotales(int m) { metros_totales = m; }
            
        void move();
        
    private:
        int id;
        std::string name;
        int position;
        char symbol;
        int vueltas_actuales;
        int metros_totales;

};

#endif