/*
 * Copyright (C) 2013- yeyouqun@163.com
 */ 
#ifndef __XDELTA_LIB_H__
#define __XDELTA_LIB_H__

/// @file
/// @mainpage xdeltalib ��ʹ��ָ��
///
/// @section intro_sec ����
/// xdeltalib ������ C++ ʵ�ֵĲ���������ȡ�⣬����ľ��� Rsync ���㷨��
/// Xdeltalib ����ص������¼�����
/// \li ��ȫ�� C++ д�ɣ����Լ��е� C++ ��Ŀ�У���������� C++ �����ơ�
/// \li ֧�ֶ�ƽ̨���� Windows �� Linux �о����ϸ���ԣ�Ҳ�������ϵ� Unix ƽ̨�С�
/// \li ���뾭���ر��Ż��������㷨�����ݽṹ����������ƣ�������ִ�����ܡ�
/// \li ֧�� in-place ͬ���㷨������Ӧ�õ�����ƽ̨�У������ƶ�ƽ̨�������������Լ� PC ������
/// \li ֧�ֿ����õ� multi-round �����֣�ͬ���㷨�����ͬ��Ч�ʣ�ͬʱ����˼���ƽ̨�Ŀ������ԡ�
/// \li �����������ݴ��书�ܣ��������û����ϵĹ��������ӿ����Ͻ��ȡ�
/// \li ֧�ֿ����õĻ���Ĭ�ϵ��߳�����������ö�����ƣ������ִ�����ܡ�
/// \li ������������������ģ���ύ�봦�����񣬳�����ò������ơ�
/// \li һ�����;���������ڴ�ͳ���ļ�����ͬ����Ҳ���������������㷨��Ӧ�õĳ�����
/// \li ���õ�ƽ̨��Ӧ�ԡ�ͨ���ر����ƣ��ṩ�ڸ��ִ洢ƽ̨��Ӧ�ã��絥�豸�������ƴ洢�������Լ��ִ�ʽ�洢������
/// \li �걸���ĵ���֧���������Ӧ��
///
/// @section port_sec ����ֲ
/// xdeltalib ��������õĿ���ֲ�ԣ�����ʹ���� Windows ƽ̨��Linux ƽ̨����������ƽ̨�Ͻ�����
/// ȫ��Ĳ��ԣ�ֻҪ�����ٵĹ�����������ֲ�� Unix ƽ̨��
///
/// @section class_sec ������˵��
/// xdeltalib.h �ļ��ṩ�� xdeltalib ��ĺ�������Ļ�������������
/// \li hasher_stream       ���������ļ� Hash ֵ�Ļ��ࡣ
/// \li xdelta_stream       ������������ Hash �����ɵĲ������ݵĻ��࣬�û�����ʵ���Լ��Ļ��࣬��
///                         ��ʵ��ʹ�ò������ݵĹ��ܡ�
/// \li hash_table          ���ڼ�¼ Hash �����������ڵ�����ѯ�����롣
/// \li multiround_hash_table ���ڼ�¼ Hash �����������ڵ�����ѯ�����룬��ֻ���ڶ��� Hash �С�
/// \li xdelta_hash_table   ����ʵ�ֲ������ݵļ��㣬��ͨ�� xdelta_stream ���������
/// \li rolling_hasher      ʵ�� Rolling Hash ���ࡣ
///
/// @section misc_sec ����˵��
/// \li ��ͳ�Ŀ����ļ�����ͬ�����������ػ���Զ�̡�
/// \li ����Դ���ظ�����ɾ����
/// \li �����ƴ洢�������̲�Ʒ���������ݵ�ͬ����ȥ�ء�
/// \li �Բ������ݵķ�ʽ�����ļ���汾��
/// \li�ȵȡ�


/// \namespace xdelta
/// \brief ��������ֿռ� xdelta
/// xdeltalib ���е�ʵ�־��ڴ����ֿռ��С�
namespace xdelta {

/// \struct
/// ��������Ŀ���ļ���λ����Ϣ
struct target_pos
{
	uint64_t	t_offset;		///< Hash ֵ��Ŀ���ļ����ļ���index �������ֵ����ʾ��
								///< ������ֻ�ڶ��� Hash ʱ�Ż��õ������������Ϊ 0��
	uint64_t	index;          ///< Hash ���ݿ�Ŀ�����ֵ��
};

/// \struct
/// ���������� Hash ֵ��
struct slow_hash {
    uchar_t		hash[DIGEST_BYTES]; ///< ���ݿ�� MD4 Hash ֵ��
	target_pos	tpos;				///< ���ݿ���Ŀ���ļ��е�λ����Ϣ��
};

/// \struct
/// �ڶ��� Hash ���������������ļ������� Hash ���������ļ�����
struct hole_t
{
	uint64_t offset;		///< �ļ���ƫ�ơ�
	uint64_t length;		///< �ļ����ĳ��ȡ�
};

} // namespace xdelta

NAMESPACE_STD_BEGIN

template <> struct less<xdelta::hole_t> {
	bool operator () (const xdelta::hole_t & left, const xdelta::hole_t & right) const
	{
		return right.offset + right.length < left.offset;
	}
};

template <> struct less<xdelta::slow_hash> {
	bool operator () (const xdelta::slow_hash & left, const xdelta::slow_hash & right) const
	{
		return memcmp (left.hash, right.hash, DIGEST_BYTES) < 0 ? true : false;
	}
};

NAMESPACE_STD_END

namespace xdelta {

class DLL_EXPORT xdelta_stream 
{
public:
	virtual ~xdelta_stream () {} 
	/// \brief
	/// ָʾ��ʼ�����ļ��� Hash ��
	/// \param[in] fname	�ļ����������·��
	/// \param[in] blk_len	�����ļ��Ŀ鳤��
	/// \return û�з���
	virtual void start_hash_stream (const std::string & fname, const int32_t blk_len) = 0;
	/// \brief
	/// ���һ����ͬ��Ŀ���Ϣ��¼
	/// \param[in] tpos		����Ŀ���ļ��е�λ����Ϣ��
	/// \param[in] blk_len	�鳤�ȡ�
	/// \param[in] s_offset	��ͬ����Դ�ļ��е�λ��ƫ�ơ�
	/// \return û�з���
	virtual void add_block (const target_pos & tpos
							, const uint32_t blk_len
							, const uint64_t s_offset) = 0;
	/// \brief
	/// ���һ������Ŀ����ݵ����С�
	/// \param[in] data		�������ݿ�ָ�롣
	/// \param[in] blk_len	���ݿ鳤�ȡ�
	/// \param[in] s_offset	���ݿ���Դ�ļ��е�λ��ƫ�ơ�
	/// \return û�з���
	virtual void add_block (const uchar_t * data
							, const uint32_t blk_len
							, const uint64_t s_offset) = 0;
	/// \brief
	/// ָʾ��һ�� Hash ����ʼ��
	/// \param[in] blk_len		��һ�� Hash �Ŀ鳤�ȡ�
	/// \return û�з���
	virtual void next_round (const int32_t blk_len) = 0;
	/// \brief
	/// ָʾ����һ�� Hash��ֻ�ڶ��� Hash �е���
	/// \return û�з���
	virtual void end_one_round () = 0;
	/// \brief
	/// ָʾ����һ���ļ��� Hash ���Ĵ�����
	/// \param[in] filsize		Դ�ļ��Ĵ�С��
	/// \return û�з���
	virtual void end_hash_stream (const uint64_t filsize) = 0;
	/// \brief
	/// ָʾ���������еĴ���
	/// \param[in] errmsg		������Ϣ��
	/// \param[in] errorno		�����롣
	/// \return û�з���
	virtual void on_error (const std::string & errmsg, const int errorno) = 0;
	/// \brief
	/// �ڶ��� Hash ������Դ�ļ�������
	/// \param[in] holeset		�ļ������϶���
	/// \return û�з���
	virtual void set_holes (std::set<hole_t> * holeset) = 0;
};

class DLL_EXPORT hasher_stream 
{
public:
	virtual ~hasher_stream () {}
	/// \brief
	/// ָʾ��ʼ�����ļ��� Hash ��
	/// \param[in] fname	�ļ����������·��
	/// \param[in] blk_len	�����ļ��Ŀ鳤��
	/// \return û�з���
	virtual void start_hash_stream (const std::string & fname, const int32_t blk_len) = 0;
	/// \brief
	/// ���һ�������ݵĿ졢�� Hash ֵ��
	/// \param[in] fhash		�� Hash ֵ��
	/// \param[in] shash		�� Hash ֵ��
	virtual void add_block (const uint32_t fhash, const slow_hash & shash) = 0;
	/// \brief
	/// ָʾ����һ���ļ��� Hash ���Ĵ�����
	/// \param[in] filsize		Դ�ļ��Ĵ�С��
	/// \return û�з���
	virtual void end_hash_stream (const uchar_t file_hash[DIGEST_BYTES], const uint64_t filsize) = 0;
	/// \brief
	/// ָʾ�������� Hash �е�һ�֣�����Ӧ����൱�ڵ��� Hash �е� end_hash_stream��
	/// \param[in] file_hash		�����ļ��� MD4 Hash ֵ��
	/// \return ���Դ�ļ����ж���Ҫ������һ�֣��򷵻��棬���򷵻ؼ١�
	virtual bool end_first_round (const uchar_t file_hash[DIGEST_BYTES]) = 0;
	/// \brief
	/// ָʾ��һ�� Hash ����ʼ��
	/// \param[in] blk_len		��һ�� Hash �Ŀ鳤�ȡ�
	/// \return û�з���
	virtual void next_round (const int32_t blk_len) = 0;
	/// \brief
	/// ָʾ����һ�� Hash��ֻ�ڶ��� Hash �е���
	/// \return û�з���
	virtual void end_one_round () = 0;
	/// \brief
	/// ָʾ���������еĴ���
	/// \param[in] errmsg		������Ϣ��
	/// \param[in] errorno		�����롣
	/// \return û�з���
	virtual void on_error (const std::string & errmsg, const int errorno) = 0;
	/// \brief
	/// ����� Hash ������Ŀ���ļ�������
	/// \param[in] holeset		�ļ������϶���
	/// \return û�з���
	virtual void set_holes (std::set<hole_t> * holeset) = 0;
};

/// �ڵ��� Hash �е���С�鳤��
#define XDELTA_BLOCK_SIZE 400

/// �ڵ��� Hash �е����鳤��
#define MAX_XDELTA_BLOCK_BYTES ((int32_t)1 << 17) //128KB

/// �ڶ��� Hash �е����鳤��
#define MULTIROUND_MAX_BLOCK_SIZE (1 << 22)

/// ����ʹ�û��泤��
#define XDELTA_BUFFER_LEN ((int32_t)1 << 25) // 32MB

#define MAX(a,b) ((a)>(b)?(a):(b))
    
/// \fn int32_t minimal_multiround_block
/// \brief ȡ���ڶ��� Hash ����С�Ŀ鳤��, refer to:<<Multiround Rsync.pdf>> to get
/// more details for multiround rsync.
inline int32_t minimal_multiround_block ()
{
	return XDELTA_BLOCK_SIZE;
}

/// \fn int32_t multiround_base 
/// \brief
/// ���ض��� Hash �еĿ�����������Сÿ�γ������������refer to:<<Multiround Rsync.pdf>> to get
/// more details for multiround rsync.
inline int32_t multiround_base ()
{
#define MULTIROUND_BASE_VALUE 3
	return MULTIROUND_BASE_VALUE;
}

class DLL_EXPORT hash_table  {
	hash_map<uint32_t, std::set<slow_hash> * > hash_table_;
	typedef hash_map<uint32_t, std::set<slow_hash> *>::const_iterator chit_t;
	typedef hash_map<uint32_t, std::set<slow_hash> *>::iterator hash_iter;
public:
	hash_table () {}
	virtual ~hash_table ();
	/// \brief
	/// ������е� Hash ֵ��
	/// \return		no return.
	///
	void clear ();
	/// \brief
	/// ��� Hash ���Ƿ�Ϊ�ա�
	/// \return If hash table is empty,return true, otherwise return false
	///
	bool empty () const { return hash_table_.empty (); }
	/// \brief
	/// �� Hash ���в���һ������ Hash �ԡ�
	/// \param[in]	fhash ���� Hash ֵ��
	/// \param[in]	shash ���� Hash ֵ��
	/// \return		no return.
	///
	void add_block (uint32_t fhash, const slow_hash & shash);

	/// \brief
	/// �ڱ��в����Ƿ���ָ����� Hash �ԡ�
	/// \param[in] fhash	���� Hash ֵ��
	/// \param[in] buf		���ݿ�ָ�롣
	/// \param[in] len		���ݿ鳤��
	/// \return	���������ָ������ͬ�� Hash �ԣ��򷵻���� Hash �Ե�ָ�룬���򷵻� 0.
	const slow_hash * find_block (const uint32_t fhash
									, const uchar_t * buf
									, const uint32_t len) const;
	/// \brief
	/// �����ļ��� Hash �ԣ������������
	/// \param[in] reader	�ļ��������ݴ�����ļ������ж�ȡ���ݡ�
	/// \param[in] stream	��������
	/// \return	No return
	virtual void hash_it (file_reader & reader, hasher_stream & stream) const;
};

class DLL_EXPORT multiround_hash_table : public hash_table
{
	/// \brief ��ʼ��һ�� Hash
	/// \param[in] reader	�ļ�����
	/// \param[in] stream	Hash ������
	/// \param[in] holes	Ŀ���ļ��ĵ�ǰ��
	/// \return			���ص��ֵĿ鳤�ȣ�������� -1����ʾ����Ҫ��ִ����һ�֡�
	int32_t _haser_first_round (file_reader & reader
								, hasher_stream & stream
								, std::set<hole_t> & holes) const;
	/// \brief ��ʼ��һ�� Hash
	/// \param[in] reader	�ļ�����
	/// \param[in] stream	Hash ������
	/// \param[in] holes	Ŀ���ļ��ĵ�ǰ����
	/// \param[in] blk_len	���� Hash �Ŀ鳤�ȡ�
	/// \return			�޷���
	void _next_round (file_reader & reader
							, hasher_stream & stream
							, std::set<hole_t> & holes
							, const int32_t blk_len) const;
	/// \brief
	/// �����ļ��� Hash �ԣ������������
	/// \param[in] reader	�ļ��������ݴ�����ļ������ж�ȡ���ݡ�
	/// \param[in] stream	��������
	/// \return	No return
	virtual void hash_it (file_reader & reader, hasher_stream & stream) const;
public:
	multiround_hash_table () {}
	~multiround_hash_table () {}
};

/// \class
/// �������ڸ��ݽ��յ��� Hash ����������ļ�����ִ�в��������������ͨ�� xdelta_stream
/// ���������
class DLL_EXPORT xdelta_hash_table {
protected:
	const hash_table &	hash_table_;
	file_reader &		reader_;
	const int			f_blk_len_;
public:
	/// \brief
	/// ���� Hash �������ݼ������
	/// \param[in] table	    Ŀ��Ƚ��ļ��� Hash ����
	/// \param[in] reader	    �ļ���ȡ����
	/// \param[in] f_blk_len    �鳤�ȡ�
	xdelta_hash_table (const hash_table & table
						, file_reader & reader
						, const int f_blk_len);
	~xdelta_hash_table ();
	/// \brief
	/// �����������ݲ������
	/// \param[in] stream	��������
	/// \return	No return
	void xdelta_it (xdelta_stream & stream);
};

/// \fn uint32_t DLL_EXPORT get_xdelta_block_size (const uint64_t filesize)
/// \brief �����ļ���С������Ӧ�� Hash �鳤�ȡ�
/// \param[in] filesize �ļ��Ĵ�С��
/// \return     ��Ӧ�Ŀ鳤�ȡ�
uint32_t DLL_EXPORT get_xdelta_block_size (const uint64_t filesize);

/// \class
/// ���ڼ���� Hash ֵ��
class DLL_EXPORT rolling_hasher
{
public:
	rolling_hasher () { RollsumInit ((&sum_)); }
	/// \brief
	/// ������������Ŀ� hash ֵ��
	/// \param buf1[in] ���ݿ�ָ�롣
	/// \param len[in]  ���ݿ�ĳ��ȡ�
	/// \return     ���ݿ�Ŀ� Hash ֵ��
    static uint32_t hash(const uchar_t *buf1, uint32_t len) {
		Rollsum sum;
		RollsumInit ((&sum))
    	RollsumUpdate (&sum, buf1, len);
		return RollsumDigest ((&sum));
    }
	
	/// \brief
	/// ����������ݿ��ʼ���Խ��ڲ�״̬��׼������ Rolling Hash �ӿ� update��
	/// \param buf1[in] ���ݿ�ָ�롣
	/// \param len[in]  ���ݿ�ĳ��ȡ�
	/// \return     û�з��ء�
	void eat_hash (const uchar_t *buf1, uint32_t len)
	{
		RollsumInit ((&sum_));
#ifdef _WIN32
		std::for_each (buf1, buf1 + len
			, std::bind1st (std::mem_fun1 (&rolling_hasher::_eat), this));
#else
		std::for_each (buf1, buf1 + len
			, std::bind1st (__gnu_cxx::mem_fun1 (&rolling_hasher::_eat), this));
#endif
	}
	
	/// \brief
	/// ���ض���ǰ�Ŀ� Hash ֵ��
	/// \return     ���ض���ǰ�Ŀ� Hash ֵ��
	uint32_t hash_value () const { return RollsumDigest ((&sum_)); }
    
	/// \brief
	/// ͨ���������ֽڣ�ִ�� Rolling Hash��
	/// \param[in] outchar �����л������ֽڡ�
	/// \param[in] inchar  �����л�����ֽڡ�
	/// \return     ���ض���ǰ�Ŀ� Hash ֵ��
    uint32_t update(uchar_t outchar, uchar_t inchar) {
		RollsumRotate (&sum_, outchar, inchar);
		return RollsumDigest ((&sum_));
    } 
private:
	Rollsum sum_;
	void _eat (uchar_t inchar) { RollsumRollin ((&sum_), inchar); }
};

/// \fn void get_file_digest (file_reader & reader, uchar_t digest[DIGEST_BYTES])
/// \brief ȡ���ļ��� MD4 Hash ֵ��
/// \param[in] reader	    �ļ���ȡ����
/// \param[in] reader	    Hash ֵ�����ַ��
/// \return     �޷���
void DLL_EXPORT get_file_digest (file_reader & reader, uchar_t digest[DIGEST_BYTES]);

/// \struct
/// �����ͷ��û�������еĶ���
struct DLL_EXPORT deletor
{
	virtual void release (file_reader * p) = 0;
	virtual void release (hasher_stream * p) = 0;
	virtual void release (xdelta_stream * p) = 0;
	virtual void release (hash_table * p) = 0;
};

/// \fn char_buffer<char_type> & operator << (char_buffer<char_type> & buff, const slow_hash & var)
/// \brief �� slow_hash ���������� buff �С�
/// \param[in] buff char_buff ����
/// \param[in] var  Slow Hash ����
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> & operator << (char_buffer<char_type> & buff, const slow_hash & var)
{
	buff << var.tpos.index << var.tpos.t_offset;
	buff.copy (var.hash, DIGEST_BYTES);
	return buff;
}

/// \fn char_buffer<char_type> & operator >> (char_buffer<char_type> & buff, slow_hash & var)
/// \brief �� buff �з�����һ�� slow_hash ����
/// \param[in] buff char_buff ����
/// \param[out] var  Slow Hash ����
/// \return buff char_buff �����á�
template <typename char_type>
inline char_buffer<char_type> &	operator >> (char_buffer<char_type> & buff, slow_hash & var)
{
	buff >> var.tpos.index >> var.tpos.t_offset;
	memcpy (var.hash, buff.rd_ptr (), DIGEST_BYTES);
	buff.rd_ptr (DIGEST_BYTES);
	return buff;
}

/// \fn bool is_no_file_error (int32_t error_no)
/// \brief �жϴ�������Ƿ����ļ������ڵĴ�����Ŀ¼�����ڣ��ļ��������򷵻��棬���򷵻ؼ١�
/// \param[in] error_no ������롣
/// \return ��Ŀ¼�����ڣ��ļ��������򷵻��棬���򷵻ؼ١�
inline bool is_no_file_error (const int32_t error_no)
{
	if (error_no != 0 && error_no != ENOENT
#ifdef _WIN32
			&& error_no != ERROR_FILE_NOT_FOUND
			&& error_no != ERROR_PATH_NOT_FOUND
#endif
			) {
		return false;
	}
	return true;
}

} // namespace xdelta
#endif /*__XDELTA_LIB_H__*/