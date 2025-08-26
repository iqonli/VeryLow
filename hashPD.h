#include <string>
#include <windows.h>
#include <wincrypt.h>
#include <cstdio>
using namespace std;

/*Written by ChatGPT-4o and DeepSeek
by PerryDing, QQ: 34067513
	计算哈希值的统一函数，支持多种哈希算法，根据file参数决定输入类型
	@param data: 若file为false则是输入字符串，若file为true则是文件路径
	@param file: 输入类型标识，true表示处理文件内容，false表示处理字符串内容
	@param algId: 哈希算法ID（默认SHA-256，常见算法ID如下）：
		CALG_SHA_256: SHA-256 256位
		CALG_SHA1: SHA-1 160位
		CALG_SHA_512: SHA-512 512位
		CALG_MD5: MD5 128位
@return: 十六进制字符串形式的哈希值，错误时返回错误信息*/
std::string PD_hash(const std::string& data, bool file, DWORD algId = CALG_SHA_256) 
{
	HCRYPTPROV hProv = NULL;         // 加密服务提供者句柄
	HCRYPTHASH hHash = NULL;         // 哈希对象句柄
	HANDLE hFile = INVALID_HANDLE_VALUE; // 文件句柄（仅file为true时使用）
	DWORD bytesRead = 0;             // 读取文件时的字节数
	BYTE buffer[4096];               // 文件读取缓冲区
	std::string result;              // 存储最终的十六进制哈希字符串
	
	// 输入处理分支
	if (file) 
	{
		// 以只读方式打开文件
		hFile = CreateFileA(data.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) 
		{
			return "Error: Unable to open file.";
		}
	}
	
	// 获取加密服务提供者
	// 获取PROV_RSA_AES类型的加密服务提供者，CRYPT_VERIFYCONTEXT用于简化上下文获取（无需真实证书）
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) 
	{
		// 错误处理：若操作文件则先关闭文件句柄
		if (file && hFile != INVALID_HANDLE_VALUE) 
		{
			CloseHandle(hFile); // 释放文件资源
		}
		return "Error: CryptAcquireContext failed.";
	}
	
	// 创建哈希对象（使用传入的算法ID）
	// 使用algId指定的算法创建哈希对象（关键扩展点：支持多种哈希算法）
	if (!CryptCreateHash(hProv, algId, 0, 0, &hHash)) 
	{
		// 错误处理：释放加密上下文并关闭文件（若有）
		CryptReleaseContext(hProv, 0); // 释放加密服务提供者
		if (file && hFile != INVALID_HANDLE_VALUE) 
		{
			CloseHandle(hFile); // 释放文件资源
		}
		return "Error: CryptCreateHash failed.";
	}
	
	// 计算哈希值（文件/字符串分支）
	if (file) 
	{
		// 文件输入模式：分块读取文件并逐步计算哈希
		while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) 
		{
			// 将读取的文件块数据添加到哈希计算中
			if (!CryptHashData(hHash, buffer, bytesRead, 0)) 
			{
				// 错误处理：释放所有相关资源
				CryptDestroyHash(hHash);   // 销毁哈希对象
				CryptReleaseContext(hProv, 0); // 释放加密上下文
				CloseHandle(hFile);        // 关闭文件句柄
				return "Error: CryptHashData failed.";
			}
		}
	} 
	else 
	{
		// 字符串输入模式：直接使用字符串数据计算哈希
		DWORD bytesToHash = static_cast<DWORD>(data.length()); // 获取字符串字节长度
		// 将字符串数据转换为BYTE数组并添加到哈希计算中
		if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(data.c_str()), bytesToHash, 0)) 
		{
			// 错误处理：释放加密资源（文件模式下才需要关闭文件，此处无需处理文件）
			CryptDestroyHash(hHash);   // 销毁哈希对象
			CryptReleaseContext(hProv, 0); // 释放加密上下文
			if (file && hFile != INVALID_HANDLE_VALUE) 
			{
				CloseHandle(hFile); // 防御性代码：理论上file为false时hFile是INVALID_HANDLE_VALUE
			}
			return "Error: CryptHashData failed.";
		}
	}
	
	// 获取哈希值长度
	// 查询哈希值的原始字节长度（不同算法长度不同，如SHA-256为32字节，MD5为16字节）
	DWORD hashSize = 0;
	DWORD sizeLen = sizeof(hashSize);
	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&hashSize, &sizeLen, 0)) 
	{
		// 错误处理：释放资源并返回错误信息
		CryptDestroyHash(hHash);   // 销毁哈希对象
		CryptReleaseContext(hProv, 0); // 释放加密上下文
		if (file && hFile != INVALID_HANDLE_VALUE) 
		{
			CloseHandle(hFile); // 关闭文件句柄
		}
		return "Error: CryptGetHashParam failed.";
	}
	
	// 获取实际哈希字节数据
	// 分配内存存储原始哈希字节数据
	BYTE* hashValue = new BYTE[hashSize];
	if (!CryptGetHashParam(hHash, HP_HASHVAL, hashValue, &hashSize, 0)) 
	{
		// 错误处理：释放动态分配内存及其他资源
		delete[] hashValue;          // 释放哈希字节数组
		CryptDestroyHash(hHash);     // 销毁哈希对象
		CryptReleaseContext(hProv, 0); // 释放加密上下文
		if (file && hFile != INVALID_HANDLE_VALUE) 
		{
			CloseHandle(hFile); // 关闭文件句柄
		}
		return "Error: CryptGetHashParam failed.";
	}
	
	// 转换为十六进制字符串
	// 遍历每个字节，格式化为两位十六进制字符串（如0x0A → "0a"）
	for (DWORD i = 0; i < hashSize; ++i) 
	{
		char hex[3]; // 存储两位十六进制字符，+1为终止符（实际使用2字符）
		sprintf_s(hex, sizeof(hex), "%02x", hashValue[i]); // %02x确保两位小写十六进制
		result.append(hex); // 拼接至结果字符串
	}
	
	// 资源清理
	delete[] hashValue;          // 释放动态分配的哈希字节数组
	CryptDestroyHash(hHash);     // 销毁哈希对象句柄
	CryptReleaseContext(hProv, 0); // 释放加密服务提供者句柄
	if (file && hFile != INVALID_HANDLE_VALUE) 
	{
		CloseHandle(hFile); // 关闭文件句柄（仅文件模式需要）
	}
	
	return result; // 返回最终的十六进制哈希字符串
}

