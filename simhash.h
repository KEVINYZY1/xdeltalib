/**
* ʵ��ͨ�� SIMHASH �㷨���� Fingerprint��
*/

#ifndef __SIMHASH_H__
#define __SIMHASH_H__

namespace xdelta {

/// \class
/// HashType ��ʹ�õ� Hash ���ͣ�hash_len ��ʾ�ڼ��� Hash ʱ���ִ����ȣ�һ��Ϊ 2 ~ 6��
/// ������ hash_len �ᵼ�¼�����ʧ�档
template <typename HashType, int hash_len = 3>
class simhash {
public:
	simhash (file_reader * reader);
	///\brief
	/// ������������֮��ĺ������롣
	uint32_t distance (const simhash<HashType, hash_len> & sh)
	{
	    return 0;
	}
private:
	file_reader & reader_;
};

}

#endif //__SIMHASH_H__
