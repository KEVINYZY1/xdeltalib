/*
* Copyright (C) 2016- yeyouqun@163.com
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

#ifndef __XDELTALIB_C_API_H__
#define __XDELTALIB_C_API_H__
/**
 * Ϊ����Ӧ���ڲ�ͬ�Ĵ洢ƽ̨��ʹ�ã�����ڲ����ⲿ��ͨ�Ų������� PIPE ���С����е�Ҫ���������
 * ���� PIPE �����뵽���С�
 *
 *   ��ʹ�ñ���ǰ�����Ķ���˵�������ӿ���ϸ˵����
 *
 *	1. ��ɫ���壺������ļ� A �� B����Ҫ�� A ����ͬ���� B������Ҫ���� A �� B �Ĳ������ݣ���������
 *		�� B ΪĿ���ļ���A ΪԴ�ļ���
 *
 *  2. �ڶ���ͬ�����߲�������У������ó�Ŀ���ļ���Դ�ļ��ж����ͬ�Ŀ飬����Щ��ͬ�Ŀ�ᵼ���ļ��γɲ�ͬ�ġ�������
 *		�����ζ���Щ���������в�����㣬����ʾ��
 *		a����ʼʱ A �ļ��� B �ļ����㣬�����ļ�����Ĭ��Ϊһ����0���ļ���С�Ķ�������һ����ͬ�Ŀ�����Ϊ��
 *			Դ�ļ�A��  |                           |SSSSSSSSSSSSSSS|                                               |
 *							                       ^
 *							           |-----------|
 *			Ŀ���ļ�B��|     |SSSSSSSSSSSSSSS|                                                                        |
 *		b����������� a ����ʾ S ����ͬ������γ����µĶ���
 *			Դ�ļ�A��|           ��1             |SSSSSSSSSSSSSSS|          ��2                                  |
 *							                       ^
 *							           |-----------|
 *			Ŀ���ļ�B��| ��3 |SSSSSSSSSSSSSSS|                                    ��4                                 |
 *		c��������Ҫ������3���롰ͬ4�����ø�С�Ŀ�����ϣ������������ļ� A��ͬʱ�ԡ���1���롰��2�����в�����㣬�����������ش���
 *		d�����ϼ��㣬���γɸ����С�Ķ���ֱ����С�Ŀ�ﵽ����ֹͣ���㡣
 *		e��ÿһ�ּ���ʱ���Ϳɽ��������ش����������һ�ֻᴫ�ݲ��������⣬�������ֶ��Ǵ������ͬ�����ݿ��¼��Ϣ��
 *
 *  3. ���ھ͵������ļ������ܻᵼ��ͬ��������˻������˽���ϸ����Ϣ�����Բο���
 *		In-Place Rsync: File Synchronization for Mobile and Wireless Devices��
 *				David Rasch and Randal Burns Department of Computer Science Johns Hopkins University {frasch,randalg}@cs.jhu.edu��
 *
 *	4. ���߳�֧�֡��ӿ��е����нӿڶ����̰߳�ȫ�ģ�������Լ�ͨ�����̵߳ķ������ö��������
 */
 
#ifdef _WIN32
	#define PIPE_HANDLE HANDLE
#else
	#define PIPE_HANDLE int
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	
	typedef struct file_hole {
		unsigned long long pos; // �����ļ��еľ���λ�á�
		unsigned long long len; 			// ���ĳ��ȡ�
		struct file_hole * next; // ���ڵ���һ������
	}fh_t;
	
	typedef struct hash_item {
		unsigned fast_hash;
		unsigned char	 slow_hash[DIGEST_BYTES]; // 16 Bytes
		unsigned long long t_offset; //target offset;
		unsigned t_index; // block index, ���� t_offset��֮���Բ�����ô���ӵĽṹ����Ϊ������ּ���ʹ����ͬ�Ĵ��롣
		struct hash_item * next;
	}hit_t;

	#define DT_DIFF		((unsigned short)0x0)
	#define DT_IDENT	((unsigned short)0xffff)
	/**
	 *		��������Ϊ��DT_DIFF,DT_IDENT��
	 *			DT_DIFF����ʾ��Ӧ�����ݿ��ǲ���ģ�����Ϊ�������ݵĳ��ȡ��ڹ������ļ�����ʱ��
	 *					 ��Ӧ�ô�Դ�����ļ���**Դ��ʼλ��**��ʼ��ȡָ�����ȵ����ݣ���д����
	 *					 ʱ�����ļ��� **Դ��ʼλ��** ����
	 *			DT_IDENT����ʾ��Ӧ�����ݿ�����ͬ�ģ���ʼλ�ü�¼����������ͳһΪ����ʱ��ʹ�õĿ鳤�ȡ�
	 *					 �ڹ������ļ�����ʱ����Ӧ�ôӼ���ԭʼ���ϣֵ���ļ���Ŀ���ļ���**��ʼλ��** �ж�ȡ����,
	 *					 ��д����ʱ�����ļ��� **Դ��ʼλ��**��
	 */
	typedef struct xdelta_item
	{
		unsigned short type;
		unsigned long long s_offset; // item ��ָʾ�Ŀ���Դ�ļ��е�λ�ã��������ָʾ����д���ļ���λ�á�
		unsigned long long t_offset; // type == DT_DIFF����ֵ�����壬����Ϊ DT_IDENT ʱ��ָ����Ŀ���ļ�����ʼλ�á�
									 // ���ԴӸô���ȡ blklen ���ȵ�����������ͬ������ʱ�ļ���
		unsigned index;				 // ����� t_offset  ��������������СΪ blklen��
		unsigned blklen; 	// ���ݿ�ĳ��ȡ�
		struct xdelta_item * next;
	}xit_t;

	inline unsigned long long get_target_offset (xit_t * head)
	{
		return head->t_offset + head->blklen * head->index;
	}
	
	/**
	 * ȡ���ļ���С��Ӧ�Ŀ쳤��
	 * @filesize		��Ӧ�ļ���С����Ҳ���Բ���������ӿ���ȡ���Լ������ʵĿ��С��
	 */
	DLL_EXPORT unsigned xdelta_calc_block_len (unsigned long long filesize);
	
	/**
	 * ���ּ���ʱʹ�õĽӿڡ�
	 */
	/**
	 * ��ʼһ�� HASH ���㡣
	 * @blklen		�����ϣ�Ŀ鳤�ȡ����Ŀ鳤���ܳ��� MAX_XDELTA_BLOCK_BYTES(1mb)����С����С�� 
	 *				XDELTA_BLOCK_SIZE(400) �ֽڡ�
	 * @return 	����һ���ڲ�ʹ�õ����ݽṹ��ָ�룬������ calc_hash�� get_hash_result_and_free_inner_data �ӿ��С�
	 */
	DLL_EXPORT void * xdelta_start_hash (unsigned blklen);
	/**
	 * ʹ�����½ӿڼ���ָ���������Ŀ��� HASH ֵ��
	 *
	 * @ptgthole	 ������Ĺ�ϣ�������ĸ�����Ŀ���ļ�����������ǵ��ֲ�����㣬����������������ļ���0��filesize����
	 * @inner_data	 �ڲ����ݣ��ڵ��� get_hash_result_and_free_inner_data ʱ��Ҫ�õ���
	 * @return		����һ������д�Ĺܵ�����������߿�������������������Ӧ�Ķ������ݴ��ݸ���Ľӿڡ�
	 *				���ö����뱣֤�����붴��һһ��Ӧ��ϵ�������������ݴ��󣬻���δ�������Ϊ��
	 */
	DLL_EXPORT PIPE_HANDLE xdelta_run_hash (fh_t * ptgthole, void * inner_data);
	
	/**
	 * ȡ�����һ�� xdelta_calc_hash ִ��ѭ����ִ�н����
	 * @inner_data	 	�ڲ����ݣ�����ʱ calc_hash �������ڽӿڷ��غ��������ȫ�����ͷţ������߲�����ʹ�����
	 *					������нӿڵ��á�
	 * @return			��ϣ������������ͷ�����������Ҫ���ݸ� start_xdelta  ��
	 */
	DLL_EXPORT hit_t * xdelta_get_hashes_free_inner (void * inner_data);
	/**
	 * �ͷŹ�ϣ��������
	 * @head		��ϣ���ͷ��
	 *				ע�⣺���ڹ�ϣ����п��ܱȽϴ����ļ���С��������1TB���ļ��������м�����������ʹ�����
	 *					 Ӧ���ͷ�����������
	 */
	DLL_EXPORT void xdelta_free_hashes (hit_t * head);
	
	/**
	 * ���ÿ���ļ������������ʱ�����밴���²�����У�
	 *
	 *  @blklen		��ϣ������Ӧ�����ݿ�ĳ��ȡ��ڲ���Ҫ��������������������ƶ����ڡ��������ֵ
	 *				��������ͬһ���е��� xdelta_start_hash ʱ����Ĳ���һ����
	 *  @head		��ϣ���ݿ飬xdelta_get_hash_result_and_free_inner_data �ӿڷ��ص����� 
	 *				�ڽӿڷ��غ��������ȫ�����ͷţ������߲�����ʹ�����
	 *				������нӿڵ��û��߲������е����ݡ�
	 *  @return		����һ���ڲ�ʹ�õ����ݽṹ��ָ�룬���� xdelta_run_xdelta�� xdelta_get_xdelta_result_and_free_inner_data
	 *				�ӿ�ʹ�á�
	 *
	 */
	DLL_EXPORT void * xdelta_start_xdelta (hit_t * head, unsigned blklen);
	
	/**
	 * ����ȷִ���� xdelta_start_xdelta �󣬵��ñ��ӿڡ����ӿ�����ִ�в������ݼ��㡣
	 * 
	 *  @srchole	Դ�ļ��Ķ����ڶ��ּ����У��п���Դ�ļ������ɶ������
	 *  @inner_data	�ڲ����ݣ��� xdelta_start_xdelta �ӿڲ�����
	 *  @return		����һ����������д�����ľ����ʹ����ͨ����������Ҫ����������д��
	 *				���������С�д�����ݳ��ȱ����� srchole �ĳ��ȣ�����Ҫô�ǳ���������
	 *				���߻ᵼ�·����������ȷ�������û�Ҫ��֤���ݳ��ȵ�ƥ�䡣
	 */
	DLL_EXPORT PIPE_HANDLE xdelta_run_xdelta (fh_t * srchole, void * inner_data);

	/**
	 * ȡ�����һ�� xdelta_run_xdelta ִ��ѭ����ִ�н����
	 * @inner_data	 	�ڲ����ݣ�����ʱ calc_hash �������ڽӿڷ��غ��������ȫ�����ͷţ������߲�����ʹ�����
	 *					������нӿڵ��á�
	 * @return			�������ݿ�������������ͷ����������û��������������ļ��Ĺ������������;��
	 */
	DLL_EXPORT xit_t * xdelta_get_xdeltas_free_inner (void * inner_data);
	/**
	 * �ͷŲ����������
	 * @head		������ͷ��
	 *				ע�⣺���ڲ������п��ܱȽϴ����ļ���С��������1TB���ļ��������м�����������ʹ�����
	 *					 Ӧ���ͷ�����������
	 */
	DLL_EXPORT void xdelta_free_xdeltas (xit_t * head);
	
	
	/********************************************* API �ָ� *********************************************************/
	/**
	 * ���ּ���ĵ�һ�ֿ���������� �����ּ���ʱʹ�õĽӿڡ� �����ķ�ʽ���С����ּ���ֻ�Ƕ��ּ����е�һ��������
	 * �����֣����ּ���ļ����������£�
	 * 
	 *  1. ��ʼ��Ŀ����Դ�ļ��Ķ�Ϊ 0 ���ļ���С���� (0, filesize)
	 *  2. ����Ŀ���ļ����в�ͬ��Ĺ�ϣֵ xdelta_run_hash
	 *  3. ȡ�ù�ϣ��������� xdelta_start_xdelta���������ϣ�����
	 *  4. ��Դ�ļ������ж�ִ�� xdelta_run_xdelta��
	 *  5. ȡ��ִ�н�����Խ������Ӧ�á�
	 *  6. ���ּ����ж�Ŀ���ļ���Դ�ļ�ִ�� xdelta_divide_hole��Ȼ��ص� 2��������Ѿ���С�ˣ���ִ�� 5��Ȼ�������
	 *
	 *  α�������£�
	 *
	 * 			// initialize source file hole and target file hole as (0, source filesize) and (0, target filesize)
	 *			srchole = (0, source filesize);
	 *			tgthole = (0, target filesize);
	 *			blklen = origin_len;
	 *
	 *			tmpfile = create_a_temp_file ();
	 *			for (blklen >= minimal len) { // ��ִ�е��ּ���ʱ���� minimal len �趨Ϊ blklen��
	 *				void *inner_data = start_hash ();
	 *				for (each hole in tgthole) {
	 *					pipehandle = xdelta_run_hash (hole, blklen, inner_data);
	 *					write_file_data_in_this_hole_to_pipe (pipehandle);
	 *				}
	 *				hash_result = xdelta_get_hashes_free_inner (inner_data);
	 *
	 *				inner_data = xdelta_start_xdelta (hash_result);
	 *				xdelta_free_hashes (hash_result);
	 *	 
	 *				for (each hole in srchole) {
	 *					pipehandle = xdelta_run_xdeltas (blklen, hole, inner_data);
	 *					write_file_data_in_this_hole_to_pipe (pipehandle);
	 *				}
	 *				xdelta_result = xdelta_get_xdeltas_free_inner (inner_data);
	 *
	 *				for (item in xdelta_result) {
	 *					if (item.type == DT_IDENT) {
	 *						data = copy_from (tgtfile);
	 *						seek (tmpfile, block.s_offset);
	 *						write (tmpfile, data, block.len);
	 *						xdelta_divide_hole (tgthole, item.t_offset, item.blklen);
	 *						xdelta_divide_hole (srchole, item.s_offset, item.blklen);
	 *					}
	 *				}
	 *				xdelta_free_xdeltas (xdelta_result);
	 *
	 *				blklen = next_block_len; // next_block_len < blklen
	 *			}
	 * 
	 * 			for (item in xdelta_result) {
	 *				if (item.type == DT_DIFF) {
	 *					data = copy_from (srcfile);
	 *					seek (tmpfile, block.s_offset);
	 *					write (tmpfile, data, block.len);
	 *				}
	 *			}
	 *			close (tmpfile);
	 *				
	 */
	
	/**
	 * �ָ�����
	 * @head			��ͷ�����ж���ͨ�������������ӣ��п���ֵΪ�䶯��
	 * @pos				�ָ�����λ��
	 * @len				�ָ����³��ȡ�
	 * 					ע�⣺�ֿ��п��ܲ����µĶ��ṹ���µĶ��ṹ��ͨ�� malloc �����з��䣬��˵�����
	 *						Ҫ�ڽ���ʱͨ�� free ����Դ�����ͷš��û�����ĵ�һ��������ȫ�ļ��Ķ���Ҳ��Ҫ�� malloc
	 *						���з��䣬��Ϊ�ڵ�������ӿڵĹ����У��п��ܻᵼ�� head ��ɾ������ʱ����Ҫ�����µ�ͷ
	 *						ԭ����ͷ��ͨ�� free ���ͷš���Ϊ����Ҫ��֤�ʼ�� head ��ͨ�� malloc ����ġ�
	 *					ע�⣺��֮��û���ص����֣��ڲ�ֶ�ʱ��pos + len ��һ���ȵĶ�һ���ᴦ�����е�ĳ�����У�
	 *						  ������ֻ�в����ص����������������һ���Ƿ����˴���
	 */
	DLL_EXPORT void xdelta_divide_hole (fh_t ** head, unsigned long long pos, unsigned len);
	
	/**
	 * �ͷŶ�����
	 * @head ������ͷ��
	 */
	DLL_EXPORT void xdelta_free_hole (fh_t * head);
	 
	/********************************************* API �ָ� *********************************************************/
	/**
	 * Inplace �����ļ�ʱ�Ľӿڡ���Ϊ�͵������ Inplace ��ͬʱӦ�ã�����ӿ�ֻ���ͬһ�ּ��������С�
	 */
	
	/**
	 * �����͵�����ʱ�� xdelta �����
	 *
	 * @head			xdelta ����ı�ͷ��get_xdelta_result_and_free_inner_data ��ȡ��
	 *					�ڵ�������ӿں�*head ��ֵ���е����������������Ĺ�������д���ݡ�
	 *					�����ʹ�� free_xdelta_result �ͷŽ����
	 *
	 * 			����������ʱ�������������е��������ݽ������δ�������Ϊ��
	 *				for (item in head list) {
	 *					if (link.item.type == DT_IDENT)
	 *						continue;
	 *					data = copy_from (srcfile);
	 *					seek (tmpfile, link.item.s_offset);
	 *					write (tmpfile, data, link.item.len);
	 *				}
	 */
	DLL_EXPORT void xdelta_resolve_inplace (xit_t ** head);
	
#ifdef __cplusplus
}
#endif
#endif //__XDELTALIB_C_API_H__