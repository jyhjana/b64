/*
 * b64_wrapper.h
 *  Created on: 2016��3��31��
 *      Author: guyadong
 */

#ifndef INCLUDE_B64_WRAPPER_H_
#define INCLUDE_B64_WRAPPER_H_
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include "b64/encode.h"
#include "b64/decode.h"

#define _DEF_STRING(x) #x
#define DEF_TO_STRING(x) _DEF_STRING(x)
#define SOURCE_AT __FILE__ ":" DEF_TO_STRING(__LINE__)
#define ERROR_STR(msg) std::string(SOURCE_AT ":").append(msg)

#define throw_except_if_msg(except,expression,msg) \
    if(expression)\
        throw except(ERROR_STR(msg));
#define throw_except_if(except,expression) throw_except_if_msg(except,expression,#expression)
#define throw_if_msg(expression,msg) throw_except_if_msg(std::invalid_argument,expression,msg)
#define throw_if(expression) throw_except_if(std::invalid_argument,expression)

namespace base64{
/* b64�쳣�� */
class b64_exception:public std::logic_error{
    // �̳л��๹�캯��
    using std::logic_error::logic_error;
};
/* ����libb64�����ݽ���base64����
 * inputΪnullptr��sizeΪ0�׳�std::invalid_argument�쳣
 *  */
inline std::string encode(const void* input,size_t size){
    throw_if(nullptr==input||0==size)
    std::vector<char> buffer(size<<1);
    encoder ec;
    base64_init_encodestate(&ec._state);
    // countΪbase64���������ݳ���
    auto count=ec.encode(reinterpret_cast<const char*>(input),int(size),buffer.data());
    count+=ec.encode_end(buffer.data()+count);
    assert(count<=buffer.size());
    return std::string(buffer.data(),buffer.data()+count);
}
/* �����ݶ������base64���� */
template<typename T>
inline typename std::enable_if<std::is_class<T>::value,std::string>::type
encode_obj(const T& input){
    return encode(std::addressof(input),sizeof(T));
}
/* ������������base64���� */
template<typename T>
inline std::string encode_vector(const std::vector<T>& input){
    return encode(input.data(),input.size()*sizeof(T));
}

/* ����libb64��base64������ַ������н��룬���ؽ����Ķ���������
 * inputΪ���׳�std::invalid_argument�쳣
 * */
inline std::vector<uint8_t> decode(const std::string&input){
    throw_if(input.empty())
    std::vector<uint8_t> buffer(input.size());
    decoder dc;
    base64_init_decodestate(&dc._state);
    // countΪbase64���������ݳ���
    auto count=dc.decode(input.data(),int(input.size()),reinterpret_cast<char*>(buffer.data()));
    assert(count<=buffer.size());
    return std::vector<uint8_t>(buffer.data(),buffer.data()+count);
}
/* ��base64�ַ�������Ϊ�������� */
template<typename T>
inline typename std::enable_if<std::is_class<T>::value,T>::type
decode_obj(const std::string&input){
     auto decoded_data=decode(input);
     throw_except_if_msg(b64_exception,decoded_data.size()!=sizeof(T),"decode data is invalid obj for T")
     return reinterpret_cast<T&>(decoded_data.data());
}
/* ��base64�ַ�������Ϊ���� */
template<typename T>
inline std::vector<T> decode_vector(const std::string&input){
     auto decoded_data=decode(input);
     throw_except_if_msg(b64_exception,0!=decoded_data.size()%sizeof(T),"decode data is invalid obj for std::vector<T>")
     auto p=reinterpret_cast<T*>(decoded_data.data());
     return std::vector<T>(p,p+decoded_data.size()/sizeof(T));
}

} /* namespace base64 */


#endif /* INCLUDE_B64_WRAPPER_H_ */

