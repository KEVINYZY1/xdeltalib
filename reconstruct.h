// authour:yeyouqun@163.com

#ifndef __RECONSTRUCT_H__
#define __RECONSTRUCT_H__
/// @file
/// �����ع��ļ��Ļ������ͼ����塣

namespace xdelta {

/// \class reconstructor
/// �ļ��ع����ࡣ
class DLL_EXPORT reconstructor
{
protected:
	file_operator &		foperator_;			///< �ļ������������ڷ����ļ������
	file_reader *		reader_;			///< �ļ�������
	file_writer *		writer_;			///< �ļ�д����
	std::string			fname_;				///< �������ļ�����
	std::string			ftmpname_;			///< ��ʱ�ļ���������Ҫʱ���ɡ�
	char_buffer<uchar_t>	buff_;			///< ��������
public:
	reconstructor (file_operator & foperator) : foperator_ (foperator)
		, reader_ (0), writer_ (0)
		, buff_ (XDELTA_BUFFER_LEN)
	{}
	virtual ~reconstructor () {}
	/// \brief
	/// ָʾ��ʼ�����ļ��� Hash ��
	/// \param fname[in] �ļ����������·��
	/// \param blk_len[in]	�����ļ��Ŀ鳤��
	/// \return û�з���
	virtual void start_hash_stream (const std::string & fname, const int32_t blk_len);
	/// \brief
	/// ���һ����ͬ��Ŀ���Ϣ��¼
	/// \param[in] tpos		����Ŀ���ļ��е�λ����Ϣ��
	/// \param[in] blk_len	�鳤�ȡ�
	/// \param[in] s_offset	��ͬ����Դ�ļ��е�λ��ƫ�ơ�
	/// \return û�з���
	virtual void add_block (const target_pos & tpos
							, const uint32_t blk_len
							, const uint64_t s_offset);
	/// \brief
	/// ���һ������Ŀ����ݵ����С�
	/// \param[in] data		�������ݿ�ָ�롣
	/// \param[in] blk_len	���ݿ鳤�ȡ�
	/// \param[in] s_offset	���ݿ���Դ�ļ��е�λ��ƫ�ơ�
	/// \return û�з���
	virtual void add_block (const uchar_t * data, const uint32_t blk_len, const uint64_t s_offset);
	/// \brief
	/// ָʾ����һ���ļ��� Hash ���Ĵ�����
	/// \param[in] filsize		Դ�ļ��Ĵ�С��
	/// \return û�з���
	virtual void end_hash_stream (const uint64_t filsize);
	/// \brief
	/// ָʾ���������еĴ���
	/// \param[in] errmsg		������Ϣ��
	/// \param[in] errorno		�����롣
	/// \return û�з���
	virtual void on_error (const std::string & errmsg, const int errorno);
};

uint32_t seek_and_read (file_reader & reader
					, uint64_t offset
					, uint32_t length
					, uchar_t * data);
} // namespace xdelta
#endif //__RECONSTRUCT_H__