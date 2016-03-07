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
// definition for class and method for in-place synchronizing file
// author:yeyouqun@eisoo.com

#ifndef __INPLACE_H__
#define __INPLACE_H__

/// @file
/// �����͵��ع��ļ������ͼ���ص� stream ���͡�
namespace xdelta {
/// \class
/// �͵��ļ��ع���
class DLL_EXPORT in_place_reconstructor : public reconstructor
{
	/// \brief
	/// ָʾ��ʼ�����ļ��� Hash ��������������
	/// \param fname[in] �ļ����������·��
	/// \param blk_len[in]	�����ļ��Ŀ鳤��
	/// \return û�з���
	virtual void start_hash_stream (const std::string & fname, const int32_t blk_len);
	/// \brief
	/// ���һ������Ŀ����ݵ��ļ��С�
	/// \param[in] data		�������ݿ�ָ�롣
	/// \param[in] blk_len	���ݿ鳤�ȡ�
	/// \param[in] s_offset	���ݿ���Դ�ļ��е�λ��ƫ�ơ�
	/// \return û�з���
	virtual void add_block (const uchar_t * data
							, const uint32_t blk_len
							, const uint64_t s_offset);
	/// \brief
	/// ָʾ����һ���ļ����������Ĵ���
	/// \param[in] filsize		Դ�ļ��Ĵ�С��
	/// \return û�з���
	virtual void end_hash_stream(const uint64_t filsize);
	/// \brief
	/// ָʾ��������еĴ���
	/// \param[in] errmsg		������Ϣ��
	/// \param[in] errorno		�����롣
	/// \return û�з���
	virtual void on_error(const std::string & errmsg, const int errorno);
public:
	in_place_reconstructor (file_operator & foperator) : reconstructor (foperator)
	{}
	~in_place_reconstructor () {}
};

/// \struct
/// ����һ����ͬ�����ݶ����͡�
struct equal_node
{
	uint64_t	s_offset;	///< Դ�ļ��е�ƫ��
	target_pos	tpos;		///< Ŀ���ļ��е�λ����Ϣ
	void *	 	data;
	uint32_t	blength:29; ///< �鳤�ȣ������ᳬ�� MAX_XDELTA_BLOCK_BYTES ���� MULTIROUND_MAX_BLOCK_SIZE
	uint32_t	visited:1;  ///< ����������ʾ�Ŀ��Ƿ��Ѿ��������
	uint32_t	stacked:1;	///< ����������ʾ�Ŀ��Ƿ��Ѿ��ڴ���ջ�С�
	uint32_t	deleted:1;	///< ����������ʾ�Ŀ��Ƿ��Ѿ�ɾ������ѭ����������
};

/// \struct
/// ����һ����ͬ�����ݶ�����
struct diff_node
{
	uint64_t s_offset;	///< Դ�ļ��е�ƫ�ƣ����洢��Ŀ���ļ�����ͬ��λ�á�
	uint32_t blength;	///< �鳤�ȡ�
};

} // namespace xdelta

NAMESPACE_STD_BEGIN

template <> struct less<xdelta::equal_node *> {
	bool operator () (const xdelta::equal_node * left, const xdelta::equal_node * right) const
	{
		return left->tpos.index < right->tpos.index;
	}
};

NAMESPACE_STD_END

namespace xdelta {

/// \class
/// �͵� xdelta �����ࡣ
class DLL_EXPORT in_place_stream : public xdelta_stream
{
	std::list<diff_node>	diff_nodes_;
	std::list<equal_node *>	equal_nodes_;
	xdelta_stream &			output_;
	file_reader &			reader_;
	char_buffer<uchar_t>	buff_;
private:
	/// \brief
	/// ָʾ��ʼ�����ļ��� Hash ��
	/// \param[in] fname	�ļ����������·��
	/// \param[in] blk_len	�����ļ��Ŀ鳤��
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
	virtual void add_block (const uchar_t * data
							, const uint32_t blk_len
							, const uint64_t s_offset);
	/// \brief
	/// ָʾ��һ�� Hash ����ʼ��
	/// \param[in] blk_len		��һ�� Hash �Ŀ鳤�ȡ�
	/// \return û�з���
	virtual void next_round (const int32_t blk_len) { assert (false); }
	/// \brief
	/// ָʾ����һ�� Hash��ֻ�ڶ��� Hash �е���
	/// \return û�з���
	virtual void end_one_round () { assert (false); }
	/// \brief
	/// ָʾ����һ���ļ��� Hash ���Ĵ���
	/// \param[in] filsize		Դ�ļ��Ĵ�С��
	/// \return û�з���
	virtual void end_hash_stream (const uint64_t filsize);
	/// \brief
	/// �ڶ��� Hash ������Դ�ļ�������
	/// \param[in] holeset		�ļ������϶���
	/// \return û�з���
	virtual void set_holes (std::set<hole_t> * holeset) { assert (false); }
	/// \brief
	/// ָʾ��������еĴ���
	/// \param[in] errmsg		������Ϣ��
	/// \param[in] errorno		�����롣
	/// \return û�з���
	virtual void on_error (const std::string & errmsg, const int errorno);
	void _clear ();
	/// \brief
	/// ����͵ع����ļ��У���ͬ����������ϵ�������Ⱥ�˳���������
	void _calc_send ();
public:
	in_place_stream (xdelta_stream & output, file_reader & reader)
		: output_ (output)
		, reader_ (reader) 
		, buff_ (XDELTA_BUFFER_LEN){}
	~in_place_stream () { _clear (); }
};

void resolve_inplace_identical_block (std::set<equal_node *> & enode_set
									, equal_node * node
									, std::list<equal_node*> & ident_blocks
									, std::list<diff_node> * diff_blocks = 0);

} // namespace xdelta

#endif //__INPLACE_H__

