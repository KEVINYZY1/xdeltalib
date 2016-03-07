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

#ifndef __RW_H__
#define __RW_H__
/// @file
/// ƽ̨��ص��ļ���������������

namespace xdelta {
/// \class
/// \brief �ļ���ȡ���࣬����Ҫ��д���Լ�ƽ̨���ļ���ȡ�ࡣ
class DLL_EXPORT file_reader {
public:
	virtual ~file_reader () {} 
	/// \brief
	/// ���ļ�����
	/// \return û�з���
	virtual void open_file () { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// ��ȡ�ļ���
	/// \param[out] data	���ݻ�������
	/// \param[in] len		���ļ��ĳ��ȡ�
	/// \return ���ض�ȡ���ֽ�����
	virtual int read_file (uchar_t * data, const uint32_t len) 
	{ THROW_XDELTA_EXCEPTION ("Not implemented.!"); return -1 ;}
	/// \brief
	/// �ر��ļ���
	/// \return û�з���
	virtual void close_file () { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// ȡ���ļ�����
	/// \return �ļ�����
	virtual std::string get_fname () const { THROW_XDELTA_EXCEPTION ("Not implemented.!"); return std::string (); }
	/// \brief
	/// ȡ�ļ���С��
	/// \return �ļ���С
	virtual uint64_t get_file_size () const { THROW_XDELTA_EXCEPTION ("Not implemented.!"); return -1 ; }
	/// \brief
	/// �����ļ���ָ�롣
	/// \return ����ָ��λ�á�
	virtual uint64_t seek_file (const uint64_t offset, const int whence)
	 { THROW_XDELTA_EXCEPTION ("Not implemented.!"); return -1 ; }
	/// \brief
	/// �ж��ļ��Ƿ���ڡ�
	/// \return ������ڣ��򷵻� true����������ڣ������޷��򿪣��򷵻� false��
	virtual bool exist_file () const { THROW_XDELTA_EXCEPTION ("Not implemented.!"); return false; }
};
/// \class
/// \brief �ļ�д���࣬����Ҫ��д���Լ�ƽ̨���ļ�д�ࡣ
class DLL_EXPORT file_writer {
public:
	virtual ~file_writer () {} 
	/// \brief
	/// ���ļ�����
	/// \return û�з���
	/// \throw ���������ѡ�� xdelta_exception �쳣��
	virtual void open_file () { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// д�ļ���
	/// \param[out] data	���ݻ�������
	/// \param[in] len		д�ļ��ĳ��ȡ�
	/// \return ����д����ֽ�����
	virtual int write_file (const uchar_t * data, const uint32_t len) { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// �ر��ļ���
	/// \return û�з���
	virtual void close_file () { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// ȡ���ļ�����
	/// \return �ļ�����
	virtual std::string get_fname () const { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// ȡ�ļ���С��
	/// \return �ļ���С
	virtual uint64_t get_file_size () const { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// �����ļ�дָ�롣
	/// \return ����ָ��λ�á�
	virtual uint64_t seek_file (uint64_t offset, int whence) { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// �ж��ļ��Ƿ���ڡ�
	/// \return ������ڣ��򷵻� true����������ڣ������޷��򿪣��򷵻� false��
	virtual bool exist_file () const { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
	/// \brief
	/// �����ļ���С�����С���ļ�ԭ��С����ضϣ�������дʱ��������д�����ܵ����ļ�������ƽ̨����
	/// \return �޷��ء�
	virtual void set_file_size (uint64_t filszie) { THROW_XDELTA_EXCEPTION ("Not implemented.!"); }
};

/// \class
/// �����ļ��������͡�
class DLL_EXPORT f_local_freader : public file_reader {
	virtual void open_file ();
	virtual int read_file (uchar_t * data, const uint32_t len);
	virtual void close_file ();
	virtual std::string get_fname () const { return f_filename_; }
	virtual uint64_t get_file_size () const;
	virtual uint64_t seek_file (const uint64_t offset, const int whence);
	virtual bool exist_file () const;
private:
	HANDLE f_handle_;
	const std::string f_name_;
	const std::string f_path_;
	const std::string f_filename_;
public:
	f_local_freader (const std::string & path, const std::string & fname);
	f_local_freader (const std::string & fullname);
	~f_local_freader();
};

int local_read (HANDLE handle, uchar_t * data, const uint32_t len);

/// \class
/// �����ļ��������͡�
class DLL_EXPORT f_local_fwriter : public file_writer {
	virtual void open_file ();
	virtual int write_file (const uchar_t * data, const uint32_t len);
	virtual void close_file ();
	virtual std::string get_fname () const { return f_filename_; }
	virtual uint64_t get_file_size () const;
	virtual uint64_t seek_file (uint64_t offset, int whence);
	virtual bool exist_file () const;
	virtual void set_file_size (uint64_t filszie);
private:
	HANDLE f_handle_;
	const std::string f_name_;
	const std::string f_path_;
	const std::string f_filename_;
public:
	f_local_fwriter (const std::string & path, const std::string & fname);
	f_local_fwriter (const std::string & fullname);
	~f_local_fwriter();
};

/// \class
/// �ļ����������͡�
class DLL_EXPORT file_operator
{
public:
	virtual ~file_operator () {}
	/// \brief
	/// �����ļ�������
	/// \param[in] filename �ļ�����
	/// \return ���ض���ָ�롣
	virtual file_reader * create_reader (const std::string & filename) = 0;
	/// \brief
	/// �����ļ�д����
	/// \param[in] filename �ļ�����
	/// \return ���ض���ָ�롣
	virtual file_writer * create_writer (const std::string & filename) = 0;
	/// \brief
	/// �ͷ��ļ�����
	/// \param[in] reader ����ָ�롣
	/// \return û�з��ء�
	virtual void release (file_reader * reader) = 0;
	/// \brief
	/// �ͷ��ļ�����
	/// \param[in] writer ����ָ�롣
	/// \return û�з��ء�
	virtual void release (file_writer * writer) = 0;
	/// \brief
	/// �ļ�����
	/// \param[in] old		���ļ�����
	/// \param[in] newname  ���ļ�����
	/// \return û�з��ء�
	virtual void rename (const std::string & old, const std::string & newname) = 0;
	/// \brief
	/// ɾ���ļ�
	/// \param[in] filename		�ļ�����
	/// \return û�з��ء�
	virtual void rm_file (const std::string & filename) = 0; 
};

DLL_EXPORT bool exist_file (const std::string & filename);
DLL_EXPORT uint64_t tell_file_size (const std::string & filename);

/// \class
/// �����ļ��������͡�
class DLL_EXPORT f_local_creator : public file_operator
{
	const std::string path_;
public:
	f_local_creator (const std::string & path) : path_ (path) {}
	~f_local_creator () {}
	virtual file_reader * create_reader (const std::string & filename)
	{
		std::string pathname = path_ + SEPERATOR + filename;
		return new f_local_freader (path_, filename);
	}
	virtual file_writer * create_writer (const std::string & filename)
	{
		return new f_local_fwriter (path_, filename);
	}

	virtual void release (file_reader * reader)
	{
		if (reader)
			delete reader;
	}
	virtual void release (file_writer * writer)
	{
		if (writer)
			delete writer;
	}
	virtual void rename (const std::string & old, const std::string & newname)
	{
		std::string oldname = path_ + SEPERATOR + old,
			newname1 = path_ + SEPERATOR + newname;
		::remove (newname1.c_str ());
		::rename (oldname.c_str (), newname1.c_str ());
	}
	virtual void rm_file (const std::string & filename)
	{
		std::string name = path_ + SEPERATOR + filename;
#ifdef _WIN32
		DeleteFileA (name.c_str ());
#else
		unlink (name.c_str ());
#endif
	}
};

} // namespace xdelta
#endif //__RW_H__

