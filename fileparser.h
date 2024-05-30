#pragma once

#include <QList>

#include "pimpl.h"

class FileParser final
{
public:
    FileParser();
    ~FileParser();

    bool init(const QString& fileName, uint32_t lineCount);
    bool initialized() const;
    int fileSize() const;

    void clear();
    QStringList getChunk(uint32_t& bytesRead);

private:
    DECLARE_PIMPL_CLASS(FileParser)
};
