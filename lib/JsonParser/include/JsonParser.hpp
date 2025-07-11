
#pragma once
#ifndef HPP__JsonParserARSER
#define HPP__JsonParserARSER

#include <vector>
#include <list>
#include <initializer_list>

#include <string>
#include <math.h>

#include <fstream>
#include <memory>
#include <cassert>



#define _WIDTH_AUTO     -69420      // Width of the value it holds
#define _WIDTH_MAX      -69421      // Number of characters for the width: Of the maximum value/element this is applied to
#define _WIDTH_MAX_KEY  -69422      // Max len of the keys in container


namespace JsonParser {


    /**
     * A class to represent the "key-value" pair found in JSON syntax/string.
     * The class can be used as a representation for the values, for the use in json arrays.
     * 
     */
    class jsonPair {
            std::string _info_name = "JsonParser::jsonPair";


            std::string                 _value_0;
            int                         _value_10;
            float                       _value_11;
            double                      _value_12;
            std::list<jsonPair>         _value_2;   // object(/s) `{}`
            std::list<jsonPair>         _value_3;   // array `[]`
            bool                        _value_4;

            /**
             * type of the value
             * - `
             * - `0` - `string`
             * - `1` - `number`
             *  -- `10` - number: `int`
             *  -- `11` - number: `float`
             *  -- `12` - number: `double`
             * - `2` - `json object`:    {}
             * - `3` - `array`:     []
             * - `4` - `boolean`    true/false
             * - `5` - `null`
             */
            int _type   = -1;
            bool _init  = false;

            jsonPair* _parent = nullptr;
            int _siblingIndex = 0;

        public:
            bool init() const;

            /**
             * @brief Whether the instance of this class will be passed as purely value
             * @note does not necessarily mean the stored type is a json array. The only way
             * to define if value is a json array or object is via `_type` or type()` for public
             * access
             */
            bool _onlyVal = false;
            
            std::string key;

            /**
             * @brief Construct a new jsonPair object.
             * This is the default constructor.
             */
            jsonPair() = default;
            /**
             * @brief Construct a new jsonPair object
             * 
             * @param _key `std::string` of the json pair key
             * @param parent address to the parent jsonPair that this jsonPair is stored in.
             */
            jsonPair(std::string _key, jsonPair* parent=nullptr);
            /**
             * @brief Construct a new jsonPair object.
             * 
             * @param _key `std::string` of the json pair key
             * @param _value `std::string` value to store
             * @param onlyVal whether to only display/use the value of the key-value pair
             * @param parent address to the parent jsonPair that this jsonPair is stored in.
             */
            jsonPair(std::string _key, std::string _value, bool onlyVal=false, jsonPair* parent=nullptr);
            /**
             * @brief Construct a new jsonPair object.
             * 
             * @param _key `std::string` of the json pair key
             * @param _value `int` value to store
             * @param onlyVal whether to only display/use the value of the key-value pair
             * @param parent address to the parent jsonPair that this jsonPair is stored in.
             */
            jsonPair(std::string _key, int _value, bool onlyVal=false, jsonPair* parent=nullptr);
            /**
             * @brief Construct a new jsonPair object.
             * 
             * @param _key `std::string` of the json pair key
             * @param _value `float` value to store
             * @param onlyVal whether to only display/use the value of the key-value pair
             * @param parent address to the parent jsonPair that this jsonPair is stored in.
             */
            jsonPair(std::string _key, float _value, bool onlyVal=false, jsonPair* parent=nullptr);
            /**
             * @brief Construct a new jsonPair object.
             * 
             * @param _key `std::string` of the json pair key
             * @param _value `double` value to store
             * @param onlyVal whether to only display/use the value of the key-value pair
             * @param parent address to the parent jsonPair that this jsonPair is stored in.
             */
            jsonPair(std::string _key, double _value, bool onlyVal=false, jsonPair* parent=nullptr);
            /**
             * @brief Construct a new jsonPair object.
             * 
             * @param _key `std::string` of the json pair key
             * @param _value `std::initializer_list<JsonParser::jsonPair>` json object/array to store
             * @param isArray whether `_value` is a json array (=true) or a json object (=false)
             * @param parent address to the parent jsonPair that this jsonPair is stored in
             */
            jsonPair(std::string _key, std::initializer_list<jsonPair> _value, bool isArray, jsonPair* parent=nullptr);
            /**
             * @brief Construct a new jsonPair object.
             * 
             * @param _key `std::string` of the json pair key
             * @param _value `std::vector<JsonParser::jsonPair>` json object/array to store
             * @param isArray whether `_value` is a json array (=true) or a json object (=false)
             * @param parent address to the parent jsonPair that this jsonPair is stored in
             */
            jsonPair(std::string _key, std::vector<jsonPair> _value, bool isArray, jsonPair* parent=nullptr);
            /**
             * @brief Construct a new jsonPair object
             * 
             * @param _key `std::string` of the json pair key
             * @param _value `std::list<JsonParser::jsonPair>` json object/array to store
             * @param isArray whether `_value` is a jsonPair that this jsonPair is stored in
             * @param parent address to the parent jsonpair that this jsonPair is stored in
             */
            jsonPair(std::string _key, std::list<jsonPair> _value, bool isArray, jsonPair* parent=nullptr);
            /**
             * @brief Construct a new jsonPair object.
             * 
             * @param _key `std::string` of the json pair key
             * @param _value `bool` value to store
             * @param onlyVal whether to only display/use the value of the key-value pair
             * @param parent address to the parent jsonPair that this jsonPair is stored in.
             */
            jsonPair(std::string _key, bool _value, bool onlyVal=false, jsonPair* parent=nullptr);

            void append(jsonPair newPair);
            void insert(int idx, jsonPair newPair);
            void erase(int idx);

            /**
             * type of the value
             * - `
             * - `0` - `string`
             * - `1` - `number`
             *  -- `10` - number: `int`
             *  -- `11` - number: `float`
             *  -- `12` - number: `double`
             * - `2` - `json object`:    {}
             * - `3` - `array`:     []
             * - `4` - `boolean`    true/false
             * - `5` - `null`
             */
            const int type();
            /**
             * @brief `bool` for whether a json object (type==2)`{}` or json array (type==3)`[]` is stored in this jsonPair
             * 
             * @return true type of stored value is a json array or a json object
             * @return false type of stored value is not a container type and is one of `0`-`std::string`, `10`-`int`, `11`-`float`, `12`-`double`, `4`-`bool`
             */
            const bool isContainer();

            const size_t size();

            int _str_decimal_precision = 3;

            /**
             * @brief Convert the jsonPair contents into a single `std::string`
             * 
             * @param _styleOpt different options for how to style the contents (0).
             * @param _indent number of space char's `' '` to use as indent from left side
             * @param _only_value whether to only display the values
             * @param _width_key width of the key. Will extend with space char `' '` if key doesn't fill that width.
             * @param _width_val width of the value. Will extend with space char `' '` if value doesn't fill that width.
             * @param _no_key whether to not display the key(s).
             * @param _indent_pair whether to add indent from left-side on that pair (mainly the key part)
             * @return std::string const& 
             * @note (0):
             * @note  - `0` - no line-breaks/newline. All in the same line.
             * @note  - `1` - line-breaks/newline on every "level" except the "last"/"deepest" array/json-object
             * @note  - `2` - line-breaks/newline on every "level".
             * @note  - `3` - same as `1` but add newline if "last"/"deepest" "level" is a json object literal regardless if it's the "deepest" level
             */
            std::string const& toStr(
                int     _styleOpt   = 3,
                int     _indent     = 4,
                bool    _only_value = false,
                int     _width_key  = _WIDTH_AUTO,
                int     _width_val  = _WIDTH_MAX_KEY,
                bool    _no_key     = false,
                bool    _indent_pair= true
            ) const;
            mutable std::string _toStr = "";

            friend std::ostream& operator<<(std::ostream &os, const JsonParser::jsonPair& jP);

            jsonPair& operator[] (std::string _key);
            jsonPair& operator[] (int _idx);
            
            jsonPair& operator= (std::string _newVal);
            jsonPair& operator= (int _newVal);
            jsonPair& operator= (float _newVal);
            jsonPair& operator= (double _newVal);
            jsonPair& operator= (std::initializer_list<jsonPair> _newVal);
            jsonPair& operator= (std::vector<jsonPair> _newVal);
            jsonPair& operator= (std::list<jsonPair> _newVal);
            jsonPair& operator= (bool _newVal);

            std::string&            get0();
            int&                    get10();
            float&                  get11();
            double&                 get12();
            std::list<jsonPair>&    get2();
            std::list<jsonPair>&    get3();
            bool&                   get4();

            jsonPair* getParent();
            int getSiblingIdx();

            void setParent(jsonPair* parent);
            void setSiblingIndex(int idx);
            
            /**
             * @brief Boolean access function to set whether the current stored container is a
             * json object (`isArray`=false, _type=2) or a json array (`isArray`=true, _type=3)
             * 
             * @param isArray true (json array) or false (json object)
             */
            void isArray(bool isArray);
            /**
             * @brief Load and parse a json file
             * 
             * @param filename name of the file
             * @param _verbose whether to print the loaded info
             * @param _throwError whether to throw an invalid_argument error if something goes wrong. If false, then the function will return !=0
             * @return int digit for whether the json file got successfully loaded into this class. 
             *  - `0` - success
             *  - `1` - file failed to load
             *  - `2` - some other error
             */
            int loadFile(std::string filename, bool _verbose=false, bool _throwError=true);
    };
    

    class Parser {
        private:

        public:
            jsonPair _json;


            Parser() = default;
            Parser(std::string filename, bool _verbose=false);


            /**
             * @brief Load and parse a json file
             * 
             * @param filename name of the file
             * @param _verbose whether to print the loaded info
             * @param _throwError whether to throw an invalid_argument error if something goes wrong. If false, then the function will return !=0
             * @return int digit for whether the json file got successfully loaded into this class. 
             *  - `0` - success
             *  - `1` - file failed to load
             *  - `2` - some other error
             */
            int loadFile(std::string filename, bool _verbose=false, bool _throwError=true);
    };


    /**
     *  {
     *      "projects": [
     *          {
     *              "date": "",
     *              "name": "",
     *              "pos" : [0, 0],
     *              "nodes": [
     *                  {
     *                      
     *                  }
     *              ],
     *              "links": [
     *                  {
     *                      
     *                  }
     *              ]
     *          }
     *      ]
     *  }
     * 
     * 
     * 
     *  {
     *      "key" : "value",
     *      "key" : [value, value, value],
     *      "key" : `value/object`{ "key":"value", "key":"value" }
     *  }
     * 
     */



    /// ========== Helper Functions ==========
        
    template<class T>
    inline std::string formatNumber(
        T value,
        int strWidth,
        int varPrecision    = 1,
        std::string align   = "right",
        bool numberFill     = false
    ) {
        std::stringstream outStream, _temp;
        int fillZeros = 0;
        if(numberFill && align=="right") {
            _temp << std::fixed;
            _temp << std::setprecision(varPrecision) << value;
            if(static_cast<int>(_temp.str().length()) < strWidth) fillZeros = strWidth - static_cast<int>(_temp.str().length());
        }
        outStream << std::fixed;
        outStream << std::boolalpha;
        if(align=="left") outStream<<std::left;
        else if(align=="right") outStream<<std::right;
        outStream << std::setw(strWidth - fillZeros);
        if(numberFill && align=="right") outStream << std::string(fillZeros, '0');
        // outStream << (align=="left"? std::left : std::right);
        outStream << std::setprecision(varPrecision) << value;

        return outStream.str();
    }

    template<class T>
    inline std::string formatContainer(
        T _container,
        int strWidth,
        int varPrecision,
        std::string align = "right",
        bool numberFill = false,
        char openSymb   = '{',
        char closeSymb  = '}',
        char _sepSymb   = ','
    ) {
        std::string _out(1, openSymb);
        for(auto itr=_container.begin(); itr!=_container.end(); ++itr) {
            _out += formatNumber(*itr, strWidth, varPrecision, align, numberFill);
            if(itr!=--_container.end()) openSymb += _sepSymb;
        }
        return _out + closeSymb;
    }

    template<class T>
    inline std::string formatVector(
        std::vector<T>  _container,
        int             _strWidth   = 0,
        int             _precision  = 1,
        std::string     _align      = "right",
        bool            _numberFill = false,
        char            _openSymb   = '{',
        char            _closeSymb  = '}',
        char            _sepSymb    = ',',
        size_t          _startIdx   = 0,
        size_t          _endIdx     = std::string::npos
    ) {
        std::string _out(1, _openSymb);
        size_t endIdx = (_endIdx==std::string::npos? _container.size() : _endIdx);
        for(size_t i=_startIdx; i<endIdx; i++) {
            _out += formatNumber(_container[i], _strWidth, _precision, _align, _numberFill);
            if(i<endIdx-1) _out += _sepSymb;
        }
        return _out + _closeSymb;
    }

    template<class T>
    inline std::string formatContainer1(
        T _container,
        size_t contSize,
        int strWidth,
        int varPrecision,
        std::string align = "right",
        bool numberFill = false,
        char openSymb   = '{',
        char closeSymb  = '}',
        char _sepSymb   = ','
    ) {
        std::string _out(1, openSymb);
        for(size_t i=0; i<contSize; i++) {
            _out += formatNumber(_container[i], strWidth, varPrecision, align, numberFill);
            if(i<contSize-1) _out += _sepSymb;
        }
        return _out + closeSymb;
    }

};


#endif