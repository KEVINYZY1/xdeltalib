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

#ifndef __MULTIROUND_H__
#define __MULTIROUND_H__
/// @file
/// �������� Hash �е����͡�

namespace xdelta {
class multiround_hasher_stream;

/// \class
/// ���� Hash �е��ļ��ع�����
class DLL_EXPORT multiround_reconstructor : public reconstructor
{
	std::set<hole_t> *	target_hole_;	///< Ŀ���ļ�������ͬ���л᲻�Ͻ��м�����¡�
public:
	multiround_reconstructor (file_operator & foperator) 
								: reconstructor (foperator)
	{}
	void set_holes (std::set<hole_t> * holeset)
	{
		target_hole_ = holeset;
	}
	virtual ~multiround_reconstructor () {}
	/// \brief
	/// ���һ����ͬ��Ŀ���Ϣ��¼�����ļ�����һ����ͬ�����ݿ顣
	/// \param[in] tpos		����Ŀ���ļ��е�λ����Ϣ��
	/// \param[in] blk_len	�鳤�ȡ�
	/// \param[in] s_offset	��ͬ����Դ�ļ��е�λ��ƫ�ơ�
	/// \return û�з���
	virtual void add_block(const target_pos & tpos
		, const uint32_t blk_len
		, const uint64_t s_offset);
	/// \brief
	/// ���һ������Ŀ����ݵ��ļ��С�
	/// \param[in] data		�������ݿ�ָ�롣
	/// \param[in] blk_len	���ݿ鳤�ȡ�
	/// \param[in] s_offset	���ݿ���Դ�ļ��е�λ��ƫ�ơ�
	/// \return û�з���
	virtual void add_block(const uchar_t * data
							, const uint32_t blk_len
							, const uint64_t s_offset);
};

/// \class
/// ����Դ�е� Hasher Stream��
class DLL_EXPORT multiround_hasher_stream : public hasher_stream
{
	/// \brief
	/// ָʾ��ʼ�����ļ��� Hash ��
	/// \param[in] fname	�ļ����������·��
	/// \param[in] blk_len	�����ļ��Ŀ鳤��
	/// \return û�з���
	virtual void start_hash_stream (const std::string & fname, const int32_t blk_len);
	/// \brief
	/// ���һ�������ݵĿ졢�� Hash ֵ��
	/// \param[in] fhash		�� Hash ֵ��
	/// \param[in] shash		�� Hash ֵ��
	virtual void add_block (const uint32_t fhash, const slow_hash & shash);
	/// \brief
	/// ָʾ����һ���ļ��� Hash ���Ĵ���
	/// \param[in] filsize		Դ�ļ��Ĵ�С��
	/// \return û�з���
	virtual void end_hash_stream (const uchar_t file_hash[DIGEST_BYTES], const uint64_t filsize);
	/// \brief
	/// ָʾ�������� Hash �е�һ�֣�����Ӧ����൱�ڵ��� Hash �е� end_hash_stream��
	/// \param[in] file_hash		�����ļ��� MD4 Hash ֵ��
	/// \return ���Դ�ļ����ж���Ҫ������һ�֣��򷵻��棬���򷵻ؼ١�
	virtual bool end_first_round (const uchar_t file_hash[DIGEST_BYTES]);
	/// \brief
	/// ָʾ��һ�� Hash ����ʼ��
	/// \param[in] blk_len		��һ�� Hash �Ŀ鳤�ȡ�
	/// \return û�з���
	virtual void next_round (const int32_t blk_len);
	/// \brief
	/// ָʾ����һ�� Hash��ֻ�ڶ��� Hash �е���
	/// \return û�з���
	virtual void end_one_round ();
	/// \brief
	/// ָʾ��������еĴ���
	/// \param[in] errmsg		������Ϣ��
	/// \param[in] errorno		�����롣
	/// \return û�з���
	virtual void on_error (const std::string & errmsg, const int errorno);
	/// \brief
	/// �ڶ��� Hash ������Ŀ���ļ�������
	/// \param[in] holeset		�ļ������϶���
	/// \return û�з���
	virtual void set_holes (std::set<hole_t> * holeset)
	{
		output_->set_holes (holeset);
	}
private:
	xdelta_stream *		output_;		///< ���������
	file_reader &		reader_;		///< Դ�ļ�������
	deletor *			stream_deletor_;		///< ����ɾ����
	std::set<hole_t>	source_holes_;		///< Դ�ļ�����
	hash_table			hashes_;		///< ��ǰ���յ��� Hash �����
	int32_t				blk_len_;		///< ��ǰ����Ŀ鳤�ȡ�
public:
	~multiround_hasher_stream () 
	{
		if (stream_deletor_) stream_deletor_->release (output_);
	}
	multiround_hasher_stream (file_reader & reader
							, xdelta_stream * output
							, deletor * stream_deletor)
		: output_ (output)
		, reader_ (reader)
		, stream_deletor_ (stream_deletor)
		, blk_len_ (0)
	{
		;
	}
};

} // namespace xdelta

#endif //__MULTIROUND_H__

