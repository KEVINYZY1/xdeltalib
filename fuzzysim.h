/**
* ʵ��ͨ�� SIMHASH �㷨���� Fingerprint��
*/
#ifndef __FUZZY_HASH_SIM_H__
#define __FUZZY_HASH_SIM_H__

namespace xdelta {
class fuzzy {
public:
	fuzzy (file_reader & reader) : reader_ (reader) {}
	/// \brief
	/// ������������֮������ƶȣ�0 ~ 100����Ϊ 100 ʱ����ʾ��ȫ���ơ�
	int similarity (fuzzy & f);
private:
	enum { BUFLEN = 10 * 1024 * 1024, }; // 10MB
	std::string calc_digest ();
	file_reader & reader_;
};

}

#endif //__FUZZY_HASH_SIM_H__