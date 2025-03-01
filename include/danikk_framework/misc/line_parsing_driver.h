#pragma once

#include <danikk_framework/bufferized_filestream.h>

#include <thread>

namespace danikk_framework
{
	//этот класс СПЕЦИАЛЬНО сделан для парсинга одного текстового файла в другой
	//с возможностью сохранения файлового оффсета чтения.
	class LineParsingDriver
	{
	private:
		LineFileReader reader;
		LineFileWriter writer;
		FileWriter file_offset_writer;
	public:
		LineParsingDriver() = default;

		void open(const char* src_path, const char* dest_path, uint32 line_cap = 1)
		{
			String file_offset_path;
			file_offset_path << src_path << ".offset";

			size_t file_offset = 0;
			{
				FileReader reader;
				reader.open(file_offset_path);
				if(reader.isOpen())
				{
					reader.read(&file_offset, 1);
				}
			}
			reader.open(src_path);
			writer.open(dest_path);
			file_offset_writer.open(file_offset_path);

			assert(reader.isOpen());
			assert(writer.isOpen());
			assert(file_offset_writer.isOpen());

			writer.setLineCapacity(line_cap);
			reader.set(file_offset);
			writeReaderOffset();
		}

		void writeReaderOffset()
		{
			size_t size = reader.tell();
			file_offset_writer.set(0);
			file_offset_writer.write(&size);
			file_offset_writer.flush();
			logInfo("LineParsingDriver::flush");
		}

		bool read(String& line)
		{
			return reader.read(line);
		}

		LineFileWriter& operator<<(char chr)
		{
			writer << chr;

			if(writer.hasBeenFlushed())
			{
				writeReaderOffset();
			}
			return writer;
		}

		LineFileWriter& operator<<(const char* str)
		{
			writer << str;
			return writer;
		}
	};
}
