
#include <cstdlib>

#include "JsonParser.hpp"
#include "Pos2d.hpp"

bool JsonParser::jsonPair::init() const {
    return this->_init;
}

JsonParser::jsonPair* JsonParser::jsonPair::getParent() {
    return this->_parent;
}
int JsonParser::jsonPair::getSiblingIdx() {
    return this->_siblingIndex;
}

void JsonParser::jsonPair::setParent(jsonPair* parent) {
    assert(parent);
    this->_parent = parent;
}
void JsonParser::jsonPair::setSiblingIndex(int idx) {
    assert(idx>0);
    this->_siblingIndex = idx;
}

void JsonParser::jsonPair::isArray(bool isArray) {
    assert(_type==2 || _type==3);
    
    _type = (isArray? 3 : 2);
}

JsonParser::jsonPair::jsonPair(
    std::string _key,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key) {


}

JsonParser::jsonPair::jsonPair(
    std::string _key,
    std::string _value,
    bool onlyVal,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key), _onlyVal(onlyVal), _type(0) {
    _value_0    = _value;
}
JsonParser::jsonPair::jsonPair(
    std::string _key,
    int _value,
    bool onlyVal,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key), _onlyVal(onlyVal), _type(10) {
    _value_10   = _value;
}
JsonParser::jsonPair::jsonPair(
    std::string _key,
    float _value,
    bool onlyVal,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key), _onlyVal(onlyVal), _type(11) {
    _value_11   = _value;
}
JsonParser::jsonPair::jsonPair(
    std::string _key,
    double _value,
    bool onlyVal,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key), _onlyVal(onlyVal), _type(12) {
    _value_12   = _value;
}
JsonParser::jsonPair::jsonPair(
    std::string _key,
    std::initializer_list<jsonPair> _value,
    bool isArray,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key), _type((isArray? 3 : 2)) {
    if(isArray) {
        _value_3    = _value;
        _onlyVal    = true;
        for(jsonPair& _pair : _value_3) _pair.setParent(this);
    }
    else {
        _value_2    = _value;
        for(jsonPair& _pair : _value_2) _pair.setParent(this);
    }
    
}
JsonParser::jsonPair::jsonPair(
    std::string _key,
    std::vector<jsonPair> _value,
    bool isArray,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key), _type((isArray? 3 : 2)) {
    if(isArray) {
        _value_3    = std::list<jsonPair>(_value.begin(), _value.end());
        _onlyVal    = true;
        for(jsonPair& _pair : _value_3) _pair.setParent(this);
    }
    else {
        _value_2    = std::list<jsonPair>(_value.begin(), _value.end());
        for(jsonPair& _pair : _value_2) _pair.setParent(this);
    }
}
JsonParser::jsonPair::jsonPair(
    std::string _key,
    std::list<jsonPair> _value,
    bool isArray,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key), _type((isArray? 3 : 2)) {
    if(isArray) {
        _value_3    = _value;
        _onlyVal    = true;
        for(jsonPair& _pair : _value_3) _pair.setParent(this);
    }
    else {
        _value_2    = _value;
        for(jsonPair& _pair : _value_2) _pair.setParent(this);
    }
}
JsonParser::jsonPair::jsonPair(
    std::string _key,
    bool _value,
    bool onlyVal,
    JsonParser::jsonPair* parent
): _init(true), _parent(parent), key(_key), _onlyVal(onlyVal), _type(4) {
    _value_4     = _value;
}

void JsonParser::jsonPair::append(jsonPair newPair) {
    assert(_type==2 || _type==3);
    this->insert(-1, newPair);

}
void JsonParser::jsonPair::insert(int idx, jsonPair newPair) {
    assert(_type==2 || _type==3);

    std::list<jsonPair>& lePair = (_type==2? _value_2 : _value_3);
    int _Lsize = static_cast<int>(lePair.size());
    assert(idx < _Lsize);
    if(idx<0) {
        if(idx!=-1) assert(std::abs(idx) <= _Lsize);
        idx = _Lsize += idx +1;
    }
    auto itr = lePair.begin();
    std::advance(itr, idx);
    lePair.insert(itr, newPair);
    lePair.back().setParent(this);
}
void JsonParser::jsonPair::erase(int idx) {
    assert(_type==2 || _type==3);

    std::list<jsonPair>& lePair = (_type==2? _value_2 : _value_3);
    int _Lsize = static_cast<int>(lePair.size());
    assert(idx < _Lsize);
    if(idx<0) {
        assert(std::abs(idx) <= _Lsize);
        idx = _Lsize += idx +1;
    }
    auto itr = lePair.begin();
    std::advance(itr, idx);
    lePair.erase(itr);
}

const int JsonParser::jsonPair::type() {
    return this->_type;
}

const bool JsonParser::jsonPair::isContainer() {
    return (_type==2 || _type==3);
}

const size_t JsonParser::jsonPair::size() {
    switch (_type) {
    case 2: return _value_2.size(); break;
    case 3: return _value_3.size(); break;
    default:
        assert(false);
        break;
    }
    return 0;
}

std::string const& JsonParser::jsonPair::toStr(
    int     _styleOpt,
    int     _indent,
    bool    _only_value,
    int     _width_key,
    int     _width_val,
    bool    _no_key,
    bool    _indent_pair
) const {

    //check if there are any other arrays/object-literals in any of the "elements"/values
    static auto hasCont = [](const std::list<jsonPair>& _vec) {
        for(auto el: _vec) {
            if(el.type()==2 || el.type()==3) return true;
        }
        return false;
    };


    assert(_type!=-1);
    std::string out = "";

    bool _verbose = false;

    if(_verbose) std::cout << "-------start: _type: " << _type << std::endl;

    bool val_LineB  = false;
    bool indentSub  = false;
    // bool val_LineB1 = false;
    if(_type==2 || _type==3) {
        switch (_styleOpt) {
            case 0: val_LineB = false; break;
            case 1:
                for(int _=0; _<2; _++) {
                    const std::list<JsonParser::jsonPair>& _ref = (_==0? _value_2 : _value_3);
                    if(hasCont(_ref)) {
                        val_LineB = true;
                        indentSub = true;
                        break;
                    }
                }
                break;
            case 2: val_LineB = true; indentSub = true; break;
            case 3:
                if(_type==2) { val_LineB = true; indentSub = true; break; }
                if(hasCont(_value_3)) {
                    val_LineB = true;
                    indentSub = true;
                }
                break;
        }
    }

    if(_verbose) std::cout << formatNumber<std::string>("val_LineB", 10, 0, "left")<<":" << std::boolalpha << val_LineB << std::endl;

    if(_indent_pair) out += std::string(_indent, ' ');
    if(!(_no_key || _onlyVal)) {
        int keyLen = (_width_key<0?
            (_width_key==_WIDTH_AUTO? this->key.length() : 0) :
            _width_key
        );
        out += formatNumber("\""+this->key+"\"", keyLen+2, 0, "left") + ": ";
    }
    out += (_type==2? "{" : (_type==3? "[" : ""));
    if(val_LineB) {
        out += "\n";// + std::string(_indent, ' ');
    }

    if(_verbose) std::cout << formatNumber("out: ", 10, 0, "left")<<": [\"" << out << "\"]" << std::endl;

    int len1_key    = 0;
    int len1_val    = 0;
    // if(_width_val<0) {
    //     switch (_width_val) {
    //     case _WIDTH_MAX: break;
    //     case _WIDTH_MAX_KEY:
            // if(_type==2) {
            //     std::vector<size_t> key1Len{0};

            //     for(auto el: _value_2) key1Len.push_back(el.key.length());
            //     len1_key = findVal<size_t>(key1Len, 0);
            // }
    //         break;
    //     }
    // }

    if(_type==2 || _type==3) {
        if(const_cast<JsonParser::jsonPair&>(*((_type==2? _value_2 : _value_3).begin())).type()==4) {
            // if(_width_val==_WIDTH_MAX)
            len1_val = 5; //"false"
        }
    }
    else {
        if(_verbose) {
            std::cout << formatNumber("value", 10, 0, "left")<<": ";
            switch (_type) {
                case  0: std::cout<<_value_0;  break;
                case 10: std::cout<<_value_10; break;
                case 11: std::cout<<_value_11; break;
                case 12: std::cout<<_value_12; break;
                case  4: std::cout<<std::boolalpha<<_value_4;  break;
            }
            std::cout << std::endl;
        }    
    }

    if(_verbose) std::cout << "_type: "<<_type<<": " << std::boolalpha << val_LineB << std::endl;
    switch (this->_type) {
        case 0:     out += "\""+_value_0+"\""; break;
        case 10:    out += formatNumber(_value_10, _width_val, 0); break;
        case 11:    out += formatNumber(_value_11, _width_val, _str_decimal_precision); break;
        case 12:    out += formatNumber(_value_12, _width_val, _str_decimal_precision); break;
        case 2: {
            auto itr = _value_2.begin();
            for(size_t i=0; i<_value_2.size(); i++) {
                std::string _temp = (*itr).toStr(_styleOpt, _indent, _only_value, len1_key, len1_val, false, indentSub);
                if(_verbose) std::cout << formatNumber("_temp",10,0,"left")<<": \"" <<_temp << "\"" << std::endl;
                
                if(val_LineB) {
                    out += std::string(_indent, ' ');
                    for(size_t ii=0; ii<_temp.length(); ii++) {
                        if(_temp[ii]=='\n') {
                                _temp.insert(ii+1, std::string(_indent, ' '));
                                ii += _indent*2;
                            
                        }
                    }
                }
                out += _temp;

                if(i<_value_2.size()-1) out += ",";
                if(val_LineB) { out += "\n"; }
                else if(i<_value_2.size()-1) out += " ";
                ++itr;
            }
            break;
        }
        case 3: {
            auto itr = _value_3.begin();
            for(size_t i=0; i<_value_3.size(); i++) {
                
                std::string _temp = (*itr).toStr(_styleOpt, _indent, _only_value, len1_key, len1_val, true, indentSub);
                
                if(val_LineB) {
                    out += std::string(_indent, ' ');
                    for(size_t ii=0; ii<_temp.length(); ii++) {
                        if(_temp[ii]=='\n') {
                            _temp.insert(ii+1, std::string(_indent, ' '));
                            ii += _indent*2;
                        }
                    }
                }
                out += _temp;

                if(i<_value_3.size()-1) out += ",";
                if(val_LineB) out += "\n";
                else if(i<_value_3.size()-1) out += " ";
                ++itr;
            }
            break;
        }
        case 4:     out += formatNumber(std::string((_value_4? "true" : "false")), _width_val); break;
        default:
            break;
    }

    if(val_LineB) out += std::string(_indent, ' ');
    out += (_type==2? "}" : (_type==3? "]" : ""));


    this->_toStr = out;

    if(_verbose) std::cout << "---------------"<< formatNumber("_type: ",10,0,"left")<<": "<<_type << std::endl;

    return _toStr;
}


std::ostream& JsonParser::operator<<(std::ostream& os, const JsonParser::jsonPair& jP) {
    return os << jP.toStr();
}

JsonParser::jsonPair& JsonParser::jsonPair::operator[] (std::string _key) {
    assert(_type==2);

    for(JsonParser::jsonPair& elem: _value_2) {
        if(elem.key==_key) return elem;
    }
    std::runtime_error("ERROR: "+_info_name+"::operator[](std::string):  the key \""+_key+"\" doesn't exist in json object");
}
JsonParser::jsonPair& JsonParser::jsonPair::operator[] (int _idx) {
    assert(_type==2 || _type==3);

    std::list<JsonParser::jsonPair>& toReturn = (_type==2? _value_2 : _value_3);

    int valSize = toReturn.size();
    assert(_idx<valSize);
    if(_idx<0) {
        assert(std::abs(_idx)<=valSize);
        _idx = valSize + _idx;
    }
    auto itr = toReturn.begin();
    std::advance(itr, _idx);
    return *itr;
}

JsonParser::jsonPair& JsonParser::jsonPair::operator= (std::string _newVal) {
    assert(_init);

    switch (_type) {
        case 2: _value_2.clear(); break;
        case 3: _value_3.clear(); break;
    }
    _type = 0;
    _value_0 = _newVal;
    return *this;
}
JsonParser::jsonPair& JsonParser::jsonPair::operator= (int _newVal) {
    assert(_init);
    switch (_type) {
        case 2: _value_2.clear(); break;
        case 3: _value_3.clear(); break;
    }
    _type = 10;
    _value_10 = _newVal;
    return *this;
}
JsonParser::jsonPair& JsonParser::jsonPair::operator= (float _newVal) {
    assert(_init);
    switch (_type) {
        case 2: _value_2.clear(); break;
        case 3: _value_3.clear(); break;
    }
    _type = 11;
    _value_11 = _newVal;
    return *this;
}
JsonParser::jsonPair& JsonParser::jsonPair::operator= (double _newVal) {
    assert(_init);
    switch (_type) {
        case 2: _value_2.clear(); break;
        case 3: _value_3.clear(); break;
    }
    _type = 12;
    _value_12 = _newVal;
    return *this;
}
JsonParser::jsonPair& JsonParser::jsonPair::operator= (std::initializer_list<JsonParser::jsonPair> _newVal) {
    assert(_init);

    int _vecType;
    if(_newVal.size()>0) _vecType = ((*(_newVal.begin()))._onlyVal? 3 : 2);
    else _vecType = 2;

    if(_vecType!=_type) {
        switch (_type) {
            case 2: _value_2.clear(); break;
            case 3: _value_3.clear(); break;
        }
        _type = _vecType;
    }

    std::list<jsonPair>* _contR = (_vecType==2? &_value_2 : &_value_3);
    (*_contR) = _newVal;

    for(jsonPair& _pair : *_contR) _pair.setParent(this);

    return *this;
}
JsonParser::jsonPair& JsonParser::jsonPair::operator= (std::vector<JsonParser::jsonPair> _newVal) {
    assert(_init);

    int _vecType;
    if(_newVal.size()>0) _vecType = (_newVal[0]._onlyVal? 3 : 2);
    else _vecType = 2;

    if(_vecType!=_type) {
        switch (_type) {
            case 2: _value_2.clear(); break;
            case 3: _value_3.clear(); break;
        }
        _type = _vecType;
    }

    std::list<jsonPair>* _contR = (_vecType==2? &_value_2 : &_value_3);
    (*_contR) = std::list<jsonPair>(_newVal.begin(), _newVal.end());

    for(jsonPair& _pair : *_contR) _pair.setParent(this);

    return *this;
}
JsonParser::jsonPair& JsonParser::jsonPair::operator= (std::list<JsonParser::jsonPair> _newVal) {
    assert(_init);

    int _vecType;
    if(_newVal.size()>0) _vecType = (_newVal.front()._onlyVal? 3 : 2);
    else _vecType = 2;

    if(_vecType!=_type) {
        switch (_type) {
            case 2: _value_2.clear(); break;
            case 3: _value_3.clear(); break;
        }
        _type = _vecType;
    }

    std::list<jsonPair>* _contR = (_vecType==2? &_value_2 : &_value_3);
    (*_contR) = _newVal;

    for(jsonPair& _pair : *_contR) _pair.setParent(this);

    return *this;
}
JsonParser::jsonPair& JsonParser::jsonPair::operator= (bool _newVal) {
    assert(_init);
    switch (_type) {
        case 2: _value_2.clear(); break;
        case 3: _value_3.clear(); break;
    }
    _type = 4;
    _value_4 = _newVal;
    return *this;
}

std::string& JsonParser::jsonPair::get0() {
    assert(_type==0);
    return this->_value_0;
}
int& JsonParser::jsonPair::get10() {
    assert(_type==10);
    return this->_value_10;
}
float& JsonParser::jsonPair::get11() {
    assert(_type==11);
    return this->_value_11;
}
double& JsonParser::jsonPair::get12() {
    assert(_type==12);
    return this->_value_12;
}
std::list<JsonParser::jsonPair>& JsonParser::jsonPair::get2() {
    assert(_type==2);
    return this->_value_2;
}
std::list<JsonParser::jsonPair>& JsonParser::jsonPair::get3() {
    assert(_type==3);
    return this->_value_3;
}
bool& JsonParser::jsonPair::get4() {
    assert(_type==4);
    return this->_value_4;
}


int JsonParser::jsonPair::loadFile(
    std::string filename,
    bool _verbose,
    bool _throwError
) {
    JsonParser::jsonPair*   _curr = this;
    
    std::list<JsonParser::jsonPair>   _tempPairs;
    /**
     *  {
     *      pair0,
     *      {
     *          subPair1,
     *          subPair2,
     *      }
     *  }
     * 
     * Location of the jsonPair in `_tempPairs[i]` relative to itself in two dim format:
     *    - [0]/.x - is the "absolute" "depth"/"index", as in the value of [i]
     *    - [1]/.y - if [0] value is an array or a json object (which it has to be if another jsonPair/_tempPairs-element is indexed to it), [1] refers to the element indenx
     * 
     */
    std::vector<Pos2d<int>> _pairLoc;

    std::fstream _file;
    _file.open(filename, std::fstream::in);
    if(!_file.is_open()) {
        if(_throwError) throw std::invalid_argument("file \""+filename+"\" could not be opened");
        else return 1;
    }
    else {}

    bool    inStr       = false;    // whether the characters are inside a string
    /**
     * Type of the json pair that is currently read:
     *  - ` 0` - `std::string`
     *  - `10` - `int`
     *  - `11` - `float`
     *  - `12` - `double`
     *  - ` 2` - `json object`
     *  - ` 3` - `array`
     *  - ` 4` - `bool`
     *  - ` 5` - `null`
     * 
     * Contains all the types of the current and everything "above" jsonPair's
     */


    int absIdx      = -1; // depth of the current _curr

    /**
     * Whether a new jsonPair is to be created but hasn't been created yet.
     *  - if `=false` then the jsonPair has been created and `_curr` is updated to it.
     * 
     * @warning DOES NOT DEFINE WHETHER VALUE HAS BEEN DEFINED.
     * To check if the value is defined then check if `JsonParser::jsonPair::type()` returns -1 (this'd mean it's not defiend)
     */
    bool newPair    = true;

    // std::string Key = "";
    std::string strPiece = "";
    char c, c_prev = ' ';

    int lineCount = 0;

    while(_file.get(c)) {

        // if(isKey && inStr) {
        // }
        // if(this->isContainer()) std::cout << this->size() << std::endl;
        if(c=='\n') {
            lineCount++;
        }
        if(_verbose && !(
            (c==' ' || c=='\n') && !inStr
        )) {
            std::cout << std::endl;
            std::cout << std::setw(3) << lineCount << "  ";
            std::cout << " \"" << (c=='\n'? "\\n" : std::string(1, c)) << "\" init: ";
            std::cout << " type:"<<_curr->type();
            std::cout << " inStr:"<<std::boolalpha << inStr;
            std::cout << " newPair:"<<std::boolalpha << newPair;
        }

        if(inStr) {
            if(c=='\\') {
                _file.get(c);
                switch (c) {
                    case 'n':   strPiece += '\n';   break;
                    case '\"':  strPiece += '\"';   break;
                    case '\\':  strPiece += '\\';   break;
                    default:    strPiece += c;      break;
                }
                continue;
            }
            if(c=='\"') { //closing strPiece
                if(_verbose) std::cout << " " << std::boolalpha << newPair;
                if(newPair) { //create first instance of jsonPair WITH keys
                    // std::cout << "-- -- -- --" << _curr->type() << std::endl;


                    if(_verbose && _curr->size()>0) {
                        std::cout << " before:{";
                        std::cout << " s:" << _curr->size();
                        std::cout << " ptr:" << &(_curr->operator[](-1));
                        std::cout << " key:" << _curr->key;
                        std::cout << "}";
                    }
                    if(_curr->type()==2)_curr->append(JsonParser::jsonPair(strPiece));
                    else                _curr->append(JsonParser::jsonPair("_\"_", strPiece, true));

                    if(_verbose && _curr->size()>0) {
                        std::cout << " after:{";
                        std::cout << " s:" << _curr->size();
                        std::cout << " ptr:" << &(_curr->operator[](-1));
                        std::cout << " key:" << _curr->key;
                        std::cout << "}";
                        if(_curr->type()==2) {
                            std::cout << " all:[";
                            for(jsonPair& _ref : _curr->get2()) {
                                std::cout << &_ref << ", ";
                            }
                            std::cout << "]";
                        }
                    }
                    // else if(strPiece=="addr")
                    _curr = &_curr->operator[](-1);
                    newPair = false;
                }
                else { //a json string value has been "read"
                    *_curr = strPiece;
                    if(_verbose) std::cout <<" "<< _curr->type() << "=TYPE "<<std::endl;
                    /**
                     * `newPair` will only be set to `true` during a clear indicator of a new jsonPair, example:
                     *  - `','`
                     */
                }
                inStr = false;
                strPiece.clear();
                
                continue;
            }

            strPiece += c;
            continue;
        }
        

        static auto parseValue = [_verbose](std::string& str, jsonPair** _jp) {
            
            //main method of defining non-string, non-json-array/object to jsonPair

            if(_verbose) std::cout << "\t\t";
            if(str=="false")       **_jp = false;
            else if(str=="true")   **_jp = true;
            else {
                static std::string numberesque = ".-+0123456789";
                std::string numberStr = "";
                int numType = 10;
                int numDecimals = 0;
                for(char cc : str) {
                    for(char ccc : numberesque) {
                        if(cc == ccc) {
                            numberStr += cc;
                            if(numType!=10) numDecimals++;
                            if(cc=='.') numType = 11;
                            break;
                        }
                    }

                }
                if(numDecimals>7) numType = 12;
                if(_verbose) std::cout << "[PARSED NUMBER:"<<numberStr<<"]";
                switch (numType) {
                case 10: if(_verbose) {std::cout<<"format:"<<std::stoi(numberStr);} (**_jp) = std::stoi(numberStr); break;
                case 11: if(_verbose) {std::cout<<"format:"<<std::stof(numberStr);} (**_jp) = std::stof(numberStr); (**_jp)._str_decimal_precision = numDecimals; break;
                case 12: if(_verbose) {std::cout<<"format:"<<std::stod(numberStr);} (**_jp) = std::stod(numberStr); (**_jp)._str_decimal_precision = numDecimals; break;
                default:
                    assert(false);
                    break;
                }
            }

            if(_verbose) {
                std::cout << " val:";
                switch((**_jp).type()) {
                    case 10: std::cout << (**_jp).get10(); break;
                    case 11: std::cout << (**_jp).get11(); break;
                    case 12: std::cout << (**_jp).get12(); break;
                }
                std::cout << " ov:"<<std::boolalpha<<(**_jp)._onlyVal;
                std::cout << " "<<(*_jp)->type() << "=TYPE "<<std::endl;
            }
            str.clear();
            
        };

        switch (c) { //outside string in json object/array
            case ' ': //we ignore space characters that aren't inside strings.
            case '\n': continue; break; //we ignore newline characters that aren't inside strings
            case '\"':
                strPiece.clear();
                inStr = true;
                break;
            case '{': //create json object jsonPair
                //vec creations::
                if(newPair) {
                    /**
                     * Create the new jsonPair element in _tempPairs and update all the related variables/containers
                     * 
                     */

                    if(_init) {
                        _curr->append(JsonParser::jsonPair("_{_", {}, false));
                        _curr = &_curr->operator[](-1);
                    }
                    else {
                        *this = jsonPair("", {}, false);
                        if(_verbose) std::cout << " "<<_curr->type() << "=TYPE "<<std::endl;
                        _onlyVal = true;
                    }
                }
                else {
                    /**
                     * Update pre-existing json (_tempPairs[current])
                     */
                    *_curr = std::vector<JsonParser::jsonPair>();
                    if(_verbose) std::cout << " "<<_curr->type() << "=TYPE "<<std::endl;
                    newPair = true;
                }
                _curr->isArray(false);
                break;
            case '[': //create json array jsonPair
                //vec creations::

                if(newPair) {
                    if(_init) {
                        _curr->append(JsonParser::jsonPair("_[_", {}, true));
                        _curr = &_curr->operator[](-1);
                        if(_verbose) std::cout << " "<<_curr->type() << "=TYPE "<<std::endl;
                    }
                    else {
                        (*this) = jsonPair("", {}, true);
                        (*this)._onlyVal = true;
                    }
                }
                else {
                    *_curr = std::vector<JsonParser::jsonPair>();
                    newPair = true;
                    if(_verbose) std::cout << " "<<_curr->type() << "=TYPE "<<std::endl;
                }
                _curr->isArray(true);
                break;
            case '}': //close json object jsonPair
                if(_curr->type()==-1) {
                    if(_verbose) std::cout << " pV:\""<<strPiece<<"\"";
                    parseValue(strPiece, &_curr);
                } //a value hasn't been defined/set/given
                newPair = false;
                _curr = _curr->getParent();
                break;
            case ']': //close json array jsonPair   || (_curr->type()==3 && newPair)
                if(_curr->type()==-1) {
                    if(_verbose) std::cout << " pV:\""<<strPiece<<"\"";
                    parseValue(strPiece, &_curr);
                }
                newPair = false;
                _curr = _curr->getParent();
                break;
            case ',': //new element in either json-array or json-object
                if(_curr->type()==3 && newPair) {
                    _curr->append(JsonParser::jsonPair(""));
                    _curr = &_curr->operator[](-1);
                    newPair = true;
                }
                if(_curr->type()==-1) {
                    if(_verbose) std::cout << " pV:\""<<strPiece<<"\"";
                    parseValue(strPiece, &_curr);
                }

                _curr = _curr->getParent();
                if(_curr->type()==3) {
                    _curr->append(JsonParser::jsonPair(""));
                    _curr = &_curr->operator[](-1);
                    newPair = false;
                }
                else {
                    newPair = true;
                }

                break;
            case ':': //define jsonPair value (though must not rely on this to detect whether something is to be defined as a value)
                // isKey = false;
                break;
            default:
                strPiece += c;
                break;
        }
    }
    c_prev = c;

    return 0;
}


// JsonParser::Parser::Parser() {

// }
JsonParser::Parser::Parser(
    std::string filename,
    bool _verbose
) {

    this->loadFile(filename, _verbose);

}

int JsonParser::Parser::loadFile(
    std::string filename,
    bool _verbose,
    bool _throwError
) {
    // JsonParser::jsonPair    _json;
    JsonParser::jsonPair*   _curr = &_json;
    
    std::list<JsonParser::jsonPair>   _tempPairs;
    /**
     *  {
     *      pair0,
     *      {
     *          subPair1,
     *          subPair2,
     *      }
     *  }
     * 
     * Location of the jsonPair in `_tempPairs[i]` relative to itself in two dim format:
     *    - [0]/.x - is the "absolute" "depth"/"index", as in the value of [i]
     *    - [1]/.y - if [0] value is an array or a json object (which it has to be if another jsonPair/_tempPairs-element is indexed to it), [1] refers to the element indenx
     * 
     */
    std::vector<Pos2d<int>> _pairLoc;

    std::fstream _file;
    _file.open(filename, std::fstream::in);
    if(!_file.is_open()) {
        if(_throwError) throw std::invalid_argument("file \""+filename+"\" could not be opened");
        else return 1;
    }
    else {}

    // bool    show_key    = true;     // whether the json "container" that where this is currently located is a json object (=true) or a json array (=false)
    bool    inStr       = false;    // whether the characters are inside a string
    // // bool    ignChar     = false;    // whether the preceding character was `'\'`
    // bool    isKey       = true;     // whether the current reading is of the `key` in key-value pair
    // /**
    //  * Type of the json pair that is currently read:
    //  *  - ` 0` - `std::string`
    //  *  - `10` - `int`
    //  *  - `11` - `float`
    //  *  - `12` - `double`
    //  *  - ` 2` - `json object`
    //  *  - ` 3` - `array`
    //  *  - ` 4` - `bool`
    //  *  - ` 5` - `null`
    //  * 
    //  * Contains all the types of the current and everything "above" jsonPair's
    //  */
    // std::vector<int> pairType;

    // std::vector<JsonParser::jsonPair*>  storedAt;
    // std::vector<int> pairDepth;     // number of "levels" the current reading is stored
    // std::vector<int> pairElement;   // index to the element of the json object/array that *this* jsonPair is stored at

    int absIdx      = -1; // depth of the current _curr

    // int currDepth   = -1;
    // int currElem    =  0;
    // int currType    = -1;

    // bool isDef_key  = false;    // isDefined: key
    // bool isDef_val  = false;    // isDefined: value

    // bool pairCreat  = false;    // whether the jsonPair hass been created

    /**
     * Whether a new jsonPair is to be created but hasn't been created yet.
     *  - if `=false` then the jsonPair has been created and `_curr` is updated to it.
     * 
     * @warning DOES NOT DEFINE WHETHER VALUE HAS BEEN DEFINED.
     * To check if the value is defined then check if `JsonParser::jsonPair::type()` returns -1 (this'd mean it's not defiend)
     */
    bool newPair    = true;

    // std::string Key = "";
    std::string strPiece = "";
    char c, c_prev = ' ';

    while(_file.get(c)) {

        // if(isKey && inStr) {
        // }
        if(!(
            (c==' ' || c=='\n') && !inStr
        )) {
            std::cout << std::endl;
            std::cout << " \"" << (c=='\n'? "\\n" : std::string(1, c)) << "\" init: ";
            std::cout << " type:"<<_curr->type();
            std::cout << " inStr:"<<std::boolalpha << inStr;
            std::cout << " newPair:"<<std::boolalpha << newPair;
        }

        if(inStr) {
            if(c=='\\') {
                _file.get(c);
                switch (c) {
                    case 'n':   strPiece += '\n';   break;
                    case '\"':  strPiece += '\"';   break;
                    case '\\':  strPiece += '\\';   break;
                    default:    strPiece += c;      break;
                }
                continue;
            }
            if(c=='\"') { //closing strPiece
                std::cout << " " << std::boolalpha << newPair;
                if(newPair) { //create first instance of jsonPair WITH keys
                    // std::cout << "-- -- -- --" << _curr->type() << std::endl;


                    if(_curr->size()>0) {
                        std::cout << " before:{";
                        std::cout << " s:" << _curr->size();
                        std::cout << " ptr:" << &(_curr->operator[](-1));
                        std::cout << " key:" << _curr->key;
                        std::cout << "}";
                    }
                    if(_curr->type()==2)_curr->append(JsonParser::jsonPair(strPiece));
                    else                _curr->append(JsonParser::jsonPair("_\"_", strPiece, true));

                    if(_curr->size()>0) {
                        std::cout << " after:{";
                        std::cout << " s:" << _curr->size();
                        std::cout << " ptr:" << &(_curr->operator[](-1));
                        std::cout << " key:" << _curr->key;
                        std::cout << "}";
                        if(_curr->type()==2) {
                            std::cout << " all:[";
                            for(jsonPair& _ref : _curr->get2()) {
                                std::cout << &_ref << ", ";
                            }
                            std::cout << "]";
                        }
                    }

                    _curr = &_curr->operator[](-1);
                    if(_curr->type()==2 || _curr->type()==3) {
                        std::cout << " size:"<<_curr->size();
                    }
                    else {
                    }
                    std::cout << " VIA STR: NEW APPEND: " << _curr->type();
                    newPair = false;
                }
                else { //a json string value has been "read"
                    *_curr = strPiece;
                    std::cout <<" "<< _curr->type() << "=TYPE "<<std::endl;
                    /**
                     * `newPair` will only be set to `true` during a clear indicator of a new jsonPair, example:
                     *  - `','`
                     */
                }
                inStr = false;
                strPiece.clear();
                
                continue;
            }

            strPiece += c;
            continue;
        }
        

        static auto parseValue = [](std::string& str, jsonPair** _jp) {
            
            //main method of defining non-string, non-json-array/object to jsonPair

            std::cout << "\t\t";
            if(str=="false")       **_jp = false;
            else if(str=="true")   **_jp = true;
            else {
                static std::string numberesque = ".-+012345679";
                std::string numberStr = "";
                int numType = 10;
                int numDecimals = 0;
                for(char cc : str) {
                    for(char ccc : numberesque) {
                        if(cc == ccc) {
                            numberStr += cc;
                            if(numType!=10) numDecimals++;
                            if(cc=='.') numType = 11;
                            break;
                        }
                    }

                }
                if(numDecimals>7) numType = 12;
                std::cout << "[PARSED NUMBER:"<<numberStr<<"]";
                switch (numType) {
                case 10: std::cout<<"format:"<<std::stoi(numberStr); (**_jp) = std::stoi(numberStr); break;
                case 11: std::cout<<"format:"<<std::stof(numberStr); (**_jp) = std::stof(numberStr); (**_jp)._str_decimal_precision = numDecimals; break;
                case 12: std::cout<<"format:"<<std::stod(numberStr); (**_jp) = std::stod(numberStr); (**_jp)._str_decimal_precision = numDecimals; break;
                default:
                    assert(false);
                    break;
                }
            }

            std::cout << " val:";
            switch((**_jp).type()) {
                case 10: std::cout << (**_jp).get10(); break;
                case 11: std::cout << (**_jp).get11(); break;
                case 12: std::cout << (**_jp).get12(); break;
            }
            std::cout << " ov:"<<std::boolalpha<<(**_jp)._onlyVal;
            std::cout << " "<<(*_jp)->type() << "=TYPE "<<std::endl;
            str.clear();
            
        };

        switch (c) { //outside string in json object/array
            case ' ': //we ignore space characters that aren't inside strings.
            case '\n': continue; break; //we ignore newline characters that aren't inside strings
            case '\"':
                inStr = true;
                break;
            case '{': //create json object jsonPair
                //vec creations::
                if(newPair) {
                    // storedAt.push_back(&*(--_tempPairs.end())); //iterator to jsonPair that this new one is stored at
                    // /**
                    //  * Create the new jsonPair element in _tempPairs and update all the related variables/containers
                    //  * 
                    //  */
                    // _tempPairs.push_back(JsonParser::jsonPair("", {}, false));
                    // _pairLoc.push_back(Pos2d(currDepth, currElem)); //update where the "new" jsonPair is located (index to "parent" jsonPair)
                    // // current++;
                    // currDepth++;

                    if(_json.init()) {
                        _curr->append(JsonParser::jsonPair("_{_", {}, false));
                        _curr = &_curr->operator[](-1);
                    }
                    else {
                        _json = jsonPair("", {}, false);
                        std::cout << " "<<_curr->type() << "=TYPE "<<std::endl;
                        _json._onlyVal = true;
                        
                    }
                }
                else {
                    /**
                     * Update pre-existing json (_tempPairs[current])
                     * 
                     */
                    *_curr = std::vector<JsonParser::jsonPair>();
                    std::cout << " "<<_curr->type() << "=TYPE "<<std::endl;
                    newPair = true;
                }
                _curr->isArray(false);
                break;
            case '[': //create json array jsonPair
                //vec creations::

                if(newPair) {
                    if(_json.init()) {
                        _curr->append(JsonParser::jsonPair("_[_", {}, true));
                        _curr = &_curr->operator[](-1);
                        std::cout << " "<<_curr->type() << "=TYPE "<<std::endl;
                    }
                    else {
                        _json = jsonPair("", {}, true);
                        _json._onlyVal = true;
                    }
                }
                else {
                    *_curr = std::vector<JsonParser::jsonPair>();
                    newPair = true;
                    std::cout << " "<<_curr->type() << "=TYPE "<<std::endl;
                }
                _curr->isArray(true);

                // pairType.push_back(3);
                // isKey = false;
                break;
            case '}': //close json object jsonPair
                if(_curr->type()==-1) {
                    std::cout << " pV:\""<<strPiece<<"\"";
                    parseValue(strPiece, &_curr);
                } //a value hasn't been defined/set/given
                newPair = false;
                
                _curr = _curr->getParent();
                // pairType.erase(--pairType.end());
                // pairDepth.erase(--pairDepth.end());
                // pairElement.erase(--pairElement.end());

                break;
            case ']': //close json array jsonPair   || (_curr->type()==3 && newPair)
                if(_curr->type()==-1) {
                    std::cout << " pV:\""<<strPiece<<"\"";
                    parseValue(strPiece, &_curr);
                }

                newPair = false;

                _curr = _curr->getParent();
                // pairType.erase(--pairType.end());
                // pairDepth.erase(--pairDepth.end());
                // pairElement.erase(--pairElement.end());
                break;
            case ',': //new element in either json-array or json-object
                if(_curr->type()==-1) {
                    std::cout << " pV:\""<<strPiece<<"\"";
                    parseValue(strPiece, &_curr);
                }

                _curr = _curr->getParent();
                if(_curr->type()==3) {
                    _curr->append(JsonParser::jsonPair(""));
                    _curr = &_curr->operator[](-1);
                    newPair = false;
                }
                else {
                    newPair = true;
                }


                // pairElement.back()++;
                // isKey = (pairType.back()==3? false : true);
                break;
            case ':': //define jsonPair value (though must not rely on this to detect whether something is to be defined as a value)
                // isKey = false;
                break;
            default:
                strPiece += c;
                break;
        }
    }
    c_prev = c;
    // std::cout << _temp.type() << std::endl;
    // this->_json = _temp;
    return 0;
}
