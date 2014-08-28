/*
* Copyright (C) 2013- yeyouqun@163.com
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, visit the http://fsf.org website.
*/

#ifndef __BUFFER_H__
#define __BUFFER_H__
/// \file
/// \brief
/// ���ļ��е����ṩ Buffer ������������������£�
/// \li �ṩ�������ռ䡣
/// \li �ṩ�������ռ����
/// \li �ṩ�������ռ��������䣬�Լ����û����������Ҫ����ջ�ռ��Ϸ�������������飩��
/// �Լ��ṩ���䶯�������߷��������������Ľӿڡ�

namespace xdelta {

/// \class
/// \brief A very simple buffer class that allocates a buffer of 
/// a given type and size in the constructor and 
/// deallocates the buffer in the destructor. 
/// 
/// This class is useful everywhere where a temporary buffer 
/// is needed. 
/// T ���������� char �Լ� unsigned char �������͡�
template <class T> 
class char_buffer 
{ 
public: 
	/// \brief
	/// ����һ����СΪ size �Ļ���������
	/// \param size[in]	��������С��
	char_buffer(const std::size_t size): 
		_size(size), 
		_occupied (0),
		_bytes_read (0),
		_ptr((T*)malloc (size * sizeof (T))),
		auto_release_ (true) { ; } 

	/// \brief
	/// ����һ����СΪ size �Ļ��������󣬵����������û����й���
	/// \param size[in]	��������С��
	char_buffer(T * buff, const std::size_t size): 
		_size(size), 
		_occupied (0),
		_bytes_read (0),
		_ptr(buff),
		auto_release_ (false) { ; } 

	~char_buffer()
	{
		if (auto_release_)
			free (_ptr);
	}
	/// \brief
	/// ���ػ������Ĵ�С
	/// \return ���ػ������Ĵ�С
	std::size_t size() const { return _size; } 
	/// \brief
	/// ���ػ��������ײ�
	/// \return ���ػ��������ײ�
	T* begin() { return _ptr; }
	const T* begin() const { return _ptr; }
	/// \brief
	/// ���ػ������Ķ�ָ�룬��ȥ���������Ѿ���������ֽڡ�
	/// \return ���ػ������Ķ�ָ��
	const T* rd_ptr () const {	return _ptr + _bytes_read; }
	T * rd_ptr () {	return _ptr + _bytes_read; }
	/// \brief
	/// ���û������Ķ�ָ��
	/// \param[in] bytes ��ָ����ƶ��ֽ�����
	/// \return �޷���
	void rd_ptr (const std::size_t bytes) {	_bytes_read += bytes; }

	/// \brief
	/// ���ػ�������дָ�룬��ȥ���������Ѿ��洢�����ֽڡ�
	/// \return ���ػ������Ķ�ָ��
	const T* wr_ptr () const {	return _ptr + _occupied; }
	T * wr_ptr () {	return _ptr + _occupied; }
	/// \brief
	/// ���û�������дָ��
	/// \param[in] bytes дָ����ƶ��ֽ�����
	/// \return �޷���
	void wr_ptr (const std::size_t bytes) {	_occupied += bytes; }

	/// \brief
	/// ��������ָ��������
	/// \return �޷���
	void reset () { _occupied = 0; _bytes_read = 0; }
	/// \brief
	/// ���ػ�������β��
	/// \return ���ػ�������β��ָ��
 	T* end() { return _ptr + _size; } 
	const T* end() const  { return _ptr + _size; }
	/// \brief
	/// �򻺳����и������ݣ������ö�Ӧ��дָ�롣
	/// \param[in] data дָ����ƶ��ֽ�����
	/// \param[in] len дָ����ƶ��ֽ�����
	/// \return �޷���
	/// \throw  xdelta_exeption ����ռ䲻�㣬������ xdelta_exeption �쳣��
	void copy (const T * data, const uint32_t len)
	{
		uint32_t remain = (uint32_t)(_size - _occupied);
		if (remain < len)
			THROW_XDELTA_EXCEPTION_NO_ERRNO ("No sufficient space left on buffer.");

		memcpy (_ptr + _occupied, data, len);
		_occupied += len;
	}
	/// \brief
	/// ���ػ�������ռ�õ��ֽ���
	/// \return ���ػ�������ռ�õ��ֽ���
	uint32_t occupied () const { return (uint32_t)_occupied; }
	/// \brief
	/// ���ػ��������������ֽ���
	/// \return ���ػ��������������ֽ���
	uint32_t data_bytes () const { return (uint32_t)(wr_ptr () - rd_ptr ()); } 
	/// \brief
	/// ���ػ����������Կռ��ֽ���
	/// \return ���ػ����������Կռ��ֽ���
	uint32_t available () const { return (uint32_t)(_size - _occupied); }
private: 
	char_buffer(); 
	char_buffer(const char_buffer&); 
	char_buffer& operator = (const char_buffer&); 
 
	std::size_t _size; 
	std::size_t _occupied;
	std::size_t _bytes_read;
	T* _ptr; 
	bool		auto_release_;
};

/// \fn char_buffer<char_type> & operator << (char_buffer<char_type> & buff, uint16_t var)
/// \brief ��������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, uint16_t var)
{
	// we use little endian bytes order.
	char_type * arr = buff.wr_ptr ();
#if BYTE_ORDER == BIG_ENDIAN
	arr[0] = (char_type)(var);
	arr[1] = (char_type)(var >> 8);
#else
	memcpy (arr, &var, 2);
#endif
	buff.wr_ptr (2);
	return buff;
}

// variable to bytes.
template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, int16_t var)
{
	return buff << (uint16_t)var;
}

/// \fn char_buffer<char_type> & operator << (char_buffer<char_type> & buff, uint32_t var)
/// \brief ��������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, uint32_t var)
{
	// we use little endian bytes order.
	char_type * arr = buff.wr_ptr ();
#if BYTE_ORDER == BIG_ENDIAN
	arr[0] = (char_type)(var);
	arr[1] = (char_type)(var >> 8);
	arr[2] = (char_type)(var >> 16);
	arr[3] = (char_type)(var >> 24);
#else
	memcpy (arr, &var, 4);
#endif
	buff.wr_ptr (4);
	return buff;
}

/// \fn char_buffer<char_type> & operator << (char_buffer<char_type> & buff, uint64_t var)
/// \brief ��������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, uint64_t var)
{
	// we use little endian bytes order.
	char_type * arr = buff.wr_ptr ();
#if BYTE_ORDER == BIG_ENDIAN
	arr[0] = (char_type)(var);
	arr[1] = (char_type)(var >> 8);
	arr[2] = (char_type)(var >> 16);
	arr[3] = (char_type)(var >> 24);
	arr[4] = (char_type)(var >> 32);
	arr[5] = (char_type)(var >> 40);
	arr[6] = (char_type)(var >> 48);
	arr[7] = (char_type)(var >> 56);
#else
	memcpy (arr, &var, 8);
#endif
	buff.wr_ptr (8);
	return buff;
}

/// \fn char_buffer<char_type> & operator << (char_buffer<char_type> & buff, const std::string & var)
/// \brief ��������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, const std::string & var)
{
	// we use little endian bytes order.
	uint32_t len = (uint32_t)var.length ();
	buff << len;
	memcpy (buff.wr_ptr (), var.c_str (), len);
	buff.wr_ptr (len);
	return buff;
}

/// \fn char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, std::string & var)
/// \brief ����������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, std::string & var)
{
	// we use little endian bytes order.
	uint32_t len;
	buff >> len;
	var.clear ();
	var.assign ((char_t*)buff.rd_ptr (), len);
	buff.rd_ptr (len);
	return buff;
}

/// \fn char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, uint16_t & var)
/// \brief ����������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, uint16_t & var)
{
	// we use little endian bytes order.
	char_type * rdptr = buff.rd_ptr ();
#if BYTE_ORDER == BIG_ENDIAN
	var = ((uint16_t)rdptr [0]) | (((uint16_t)rdptr [1]) << 8);
#else
	memcpy (&var, rdptr, 2);
#endif
	buff.rd_ptr (2);
	return buff;
}

/// \fn char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, uint32_t & var)
/// \brief ����������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, uint32_t & var)
{
	// we use little endian bytes order.
	char_type * rdptr = buff.rd_ptr ();
#if BYTE_ORDER == BIG_ENDIAN
	var = ((uint32_t)rdptr [0]) | (((uint32_t)rdptr [1]) << 8)
			| (((uint32_t)rdptr [2]) << 16) | (((uint32_t)rdptr [3]) << 24);
#else
	memcpy (&var, rdptr, 4);
#endif
	buff.rd_ptr (4);
	return buff;
}

/// \fn char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, uint64_t & var)
/// \brief ����������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, uint64_t & var)
{
	// we use little endian bytes order.
	char_type * rdptr = buff.rd_ptr ();
#if BYTE_ORDER == BIG_ENDIAN
	var = ((uint64_t)rdptr [0]) | (((uint64_t)rdptr [1]) << 8)
			| (((uint64_t)rdptr [2]) << 16) | (((uint64_t)rdptr [3]) << 24)
			| (((uint64_t)rdptr [4]) << 32) | (((uint64_t)rdptr [5]) << 40)
			| (((uint64_t)rdptr [6]) << 48) | (((uint64_t)rdptr [7]) << 56);
#else
	memcpy (&var, rdptr, 8);
#endif
	buff.rd_ptr (8);
	return buff;
}

/// \enum block_type
/// \brief ������Ŀ���ļ���Դ�ļ�֮��Ŀ�����
/// xdeltalib ��ʹ�õ��� PUSH ģʽ��ͬ�����ݣ�����Դ�˷���ͬ������
enum block_type {
	BT_CLIENT_BLOCK,			///< �ͻ��˷��͵ĳ�ʼ���ݿ飬���м�¼�˿ͻ��ȴ����ӵĶ˿ںţ��ɷ������˽������ӡ�
								///< ͨ����Щ����������֮�䴫�����ݼ� Hash �����
	BT_SERVER_BLOCK,			///< �������� BT_CLIENT_BLOCK ������Ϣ�飬�����鳤�ȣ��ļ����ȡ�
	BT_CLIENT_FILE_BLOCK,		///< �ͻ��˷��͵�����ˣ������ļ���Ϣ��
	BT_XDELTA_BEGIN_BLOCK,		///< �ͻ��˷��͵�����ˣ���ʼһ���ļ���ͬ����
	BT_EQUAL_BLOCK,				///< �ͻ��˷��͵�����ˣ�һ����ͬ�Ŀ���Ϣ��
	BT_DIFF_BLOCK,				///< �ͻ��˷��͵�����ˣ�һ���������Ϣ�����ݡ�
	BT_END_FIRST_ROUND,			///< �ͻ��˷��͵�����ˣ�������� Hash �в����ж��ֽ��У��򲻻ᷢ�ͱ��顣
	BT_XDELTA_END_BLOCK,		///< �ͻ��˷��͵�����ˣ�����һ���ļ���ͬ����
	BT_HASH_BEGIN_BLOCK,		///< ����˷����ͻ��ˣ���ʼһ���ļ�����
	BT_HASH_BLOCK,				///< ����˷����ͻ��ˣ�һ�� Hash �飬��¼ Hash ֵ��
	BT_BEGIN_ONE_ROUND,			///< ����˷����ͻ��ˣ���ʼ ��һ�֡�
	BT_END_ONE_ROUND,			///< ����˷����ͻ��ˣ�����һ�֡�
	BT_HASH_END_BLOCK,			///< ����˷����ͻ��ˣ���ʼһ���ļ�����
	BT_ERROR_BLOCK = -1,
};

/// \struct
/// \brief ���ݿ�Ŀ�ͷ�ṹ
struct block_header
{
	block_header() : blk_type(BT_ERROR_BLOCK), blk_len(-1) {}
#define BLOCK_HEAD_LEN 6
	uint16_t	blk_type;		///< �����ͣ��� block_type��
	uint32_t	blk_len;		///< �鳤�ȡ�
};


/// \fn char_buffer<char_type> & operator << (char_buffer<char_type> & buff, const block_header & var)
/// \brief ��������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, const block_header & var)
{
	return buff << var.blk_type << var.blk_len;
}

/// \fn char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, block_header & var)
/// \brief ����������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, block_header & var)
{
	return buff >> var.blk_type >> var.blk_len;
}

/// �������������ͣ�ֻ����ָʾ���Ƿ�ѹ����
#define BT_COMPRESSED			'\1'
#define BT_UNCOMPRESSED			'\0'
/// \struct
/// \brief ���ݴ��͵Ŀ�ͷ��ʽ
struct trans_block_header
{
#define TRANS_BLOCK_LEN 9
	uchar_t		compressed;		///< ���Ƿ�ѹ�������ѹ����Ϊ 0xff������Ϊ 0x0��
	uint32_t	blk_len;		///< ԭʼ�鳤�ȡ�
	uint32_t	comp_blk_size;	///< ѹ��֮��Ŀ鳤�ȣ��е����ݿ�û��ѹ������ֵ�� blk_len һ�¡�
};

/// \fn char_buffer<char_type> & operator << (char_buffer<char_type> & buff, const trans_block_header & var)
/// \brief ��������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, const trans_block_header & var)
{
	*buff.wr_ptr () = var.compressed;
	buff.wr_ptr (1);
	return buff << var.blk_len << var.comp_blk_size;
}

/// \fn char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, trans_block_header & var)
/// \brief ����������������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  ����ֵ��
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, trans_block_header & var)
{
	var.compressed = *buff.rd_ptr ();
	buff.rd_ptr (1);
	return buff >> var.blk_len >> var.comp_blk_size;
}


template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, int32_t var)
{
	return buff << (uint32_t)var;
}

template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, int64_t var)
{
	return buff << (uint64_t)var;
}

// from bytes to variable.
template <typename char_type>
inline char_buffer<char_type> &	operator >> (char_buffer<char_type> & buff, int16_t & var)
{
	uint16_t tmp;
	buff >> tmp;
	var = (int16_t)tmp;
	return buff;
}
template <typename char_type>
inline char_buffer<char_type> &	operator >> (char_buffer<char_type> & buff, int32_t & var)
{
	uint32_t tmp;
	buff >> tmp;
	var = (int32_t)tmp;
	return buff;
}
template <typename char_type>
inline char_buffer<char_type> &	operator >> (char_buffer<char_type> & buff, int64_t & var)
{
	uint64_t tmp;
	buff >> tmp;
	var = (int64_t)tmp;
	return buff;
}

/// \def STACK_BUFF_LEN
/// ջ����ռ��С��Ӧ�ñ��־���С��ͬʱ������һ�����ݿ�ͷ�Ķ�ȡ��
#define STACK_BUFF_LEN 1024
/// \def DEFINE_STACK_BUFFER(name)
/// ����һ����ջ�ϵĻ�������STACK_BUFF_LEN ����̫��
#define DEFINE_STACK_BUFFER(name)								\
	uchar_t stack_space_##name[STACK_BUFF_LEN];					\
	char_buffer<uchar_t> name (stack_space_##name, STACK_BUFF_LEN)

} // namespace xdelta
#endif //__BUFFER_H__

