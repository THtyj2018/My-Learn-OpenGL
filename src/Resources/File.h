#pragma once

#include "../Utilities/String.h"
#include <cstdio>

#include "../Utilities/Pointer.h"

enum class FileAccess
{
	ReadOnly,
	WriteOnly,
	AppendOnly,
	ReadWrite,
	ReadAppend
};

enum class SeekBase
{
	Begin,
	Current,
	End
};

class File
{
public:
	static AnsiString GetRealFilePath(const AnsiString& filename);

	File(const AnsiString& filename, FileAccess access, bool binary = true);

	~File();

	bool IsOpen() const noexcept { return m_fp != nullptr; }

	const FileAccess GetAccessMode() const noexcept { return m_Access; }

	bool IsBinary() const noexcept { return m_bBinary; }

	bool Readable() const noexcept
	{
		return m_Access == FileAccess::ReadOnly || m_Access == FileAccess::ReadWrite || m_Access == FileAccess::ReadAppend;
	}

	bool Appendable() const noexcept
	{
		return m_Access == FileAccess::AppendOnly || m_Access == FileAccess::ReadAppend;
	}

	bool Writeable() const noexcept
	{
		return m_Access != FileAccess::ReadOnly;
	}

	void Seek(int offset, SeekBase base);

	/// @brief Get data size
	/// @return File data size. If file stores text data, then return value is filesize + 1 (with a '\0')
	size_t GetDataSize() const noexcept { return m_szFile; }

	char GetChar();

	/// @brief Binary only
	/// @return Binary data
	UniquePtr<uint8_t[]> ReadBinaryData();

	/// @brief Text only
	/// @return Text data
	UniquePtr<char[]> ReadTextData();

private:
	std::FILE* m_fp;
	size_t m_szFile;
	const FileAccess m_Access;
	bool m_bBinary;
};