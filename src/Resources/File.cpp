#include "File.h"

#ifdef __linux__
#include <sys/stat.h>
#endif

AnsiString File::GetRealFilePath(const AnsiString& filepath)
{
	return "./Assets/" + filepath;
}

const char* GetSTDFileMode(FileAccess access, bool binary)
{
	switch (access)
	{
	case FileAccess::ReadOnly:
		return binary ? "rb" : "r";
	case FileAccess::WriteOnly:
		return binary ? "wb" : "w";
	case FileAccess::AppendOnly:
		return binary ? "ab" : "a";
	case FileAccess::ReadWrite:
		return binary ? "wb+" : "wb";
	case FileAccess::ReadAppend:
		return binary ? "ab+" : "ab";
	default:
		return "";
	}
}

File::File(const AnsiString& filename, FileAccess access, bool binary) :
	m_Access(access), m_bBinary(binary)
{
	auto en = fopen_s(&m_fp, GetRealFilePath(filename).c_str(), GetSTDFileMode(m_Access, m_bBinary));
	if (m_fp == nullptr)
	{
		printf("Warning: Error loading file \"%s\"! Errno: %d\n", filename.c_str(), en);
		m_szFile = 0;
		return;
	}
	
#ifdef __linux__
	struct stat stat_buf;
	fstat(_fileno(m_fp), &stat_buf);
	m_szFile = stat_buf.st_size;
#else
	// TODO: windows API or C++17 filesystem
	fseek(m_fp, 0, SEEK_END);
	m_szFile = size_t(ftell(m_fp));
	fseek(m_fp, 0, SEEK_SET);
#endif
}

File::~File()
{
	if (m_fp)
		fclose(m_fp);
}

void File::Seek(int offset, SeekBase base)
{
	if (m_fp)
		fseek(m_fp, offset, static_cast<int>(base));
}

char File::GetChar()
{
	if (!m_fp || !Readable())
	{
		printf("Warning: can't reading char from file!\n");
		return '\0';
	}
	return fgetc(m_fp);
}

UniquePtr<uint8_t[]> File::ReadBinaryData()
{
	if (!m_fp || !m_bBinary || !Readable())
	{
		printf("Warning: can't read binary data from file!\n");
		return nullptr;
	}
	int pos = ftell(m_fp);
	fseek(m_fp, 0, SEEK_SET);
	UniquePtr<uint8_t[]> buffer = MakeUnique<uint8_t[]>(m_szFile);
	size_t sz = fread_s(buffer.get(), m_szFile, 1, m_szFile, m_fp);
	if (sz != m_szFile)
		printf("Warning: Unexpected read data size! Expect %zu bytes; Read %zu bytes\n", m_szFile, sz);
	fseek(m_fp, pos, SEEK_SET);
	return buffer;
}

UniquePtr<char[]> File::ReadTextData()
{
	if (!m_fp || m_bBinary || !Readable())
	{
		printf("Warning: can't read text data from file!\n");
		return nullptr;
	}
	
	int pos = ftell(m_fp);
	fseek(m_fp, 0, SEEK_SET);
	UniquePtr<char[]> buffer = MakeUnique<char[]>(m_szFile);
	size_t sz = fread_s(buffer.get(), m_szFile + 1, 1, m_szFile + 1, m_fp);
	if (sz != m_szFile + 1)
		printf("Warning: Unexpected read data size! Expect %zu bytes; Read %zu bytes\n", m_szFile + 1, sz);
	fseek(m_fp, pos, SEEK_SET);
	return buffer;
}
