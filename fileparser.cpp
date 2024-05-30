#include "fileparser.h"

#include <QFile>
#include <QRegularExpression>

struct FileParser::impl_t
{
    QFile file;
    uint32_t lineCount { 0 };
};

FileParser::FileParser()
{
    createImpl();
}

FileParser::~FileParser()
{
    clear();
}

bool FileParser::init(const QString& fileName, uint32_t lineCount)
{
    if (impl().file.isOpen()) {
        impl().file.close();
    }

    impl().file.setFileName(fileName);

    if (!impl().file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file: " << impl().file.errorString();
        return false;
    }

    impl().lineCount = lineCount;
    return true;
}

bool FileParser::initialized() const
{
    return impl().file.isOpen();
}

QStringList FileParser::getChunk(uint32_t& bytesRead)
{
    //! Finds any word in any language, words containg numbers are discarded.
    static const QRegularExpression expr("(\\b\\p{L}+\\b)", QRegularExpression::UseUnicodePropertiesOption);
    Q_ASSERT(expr.isValid());
    Q_ASSERT(initialized());

    QStringList ret;
    uint32_t lines { 0 };

    while (lines != impl().lineCount) {
        const auto line = impl().file.readLine();

        for (const auto& word : line.split(QChar::Space)) {
            const auto match = expr.match(word);

            if (match.hasMatch()) {
                //! Converting to QString for utf-8/16 support.
                auto str = QString(word).toLower();

                auto it = std::remove_if(str.begin(), str.end(), [](const QChar& c) {
                    return !c.isLetterOrNumber();
                });

                str.chop(std::distance(it, str.end()));
                ret.append(str);
            }
        }

        bytesRead += line.size();
        lines++;
    }

    return ret;
}

void FileParser::clear()
{
    if (impl().file.isOpen()) {
        impl().file.close();
    }
}

int FileParser::fileSize() const
{
    //! Closed file returns inconsistent size.
    return initialized() ? impl().file.size() : 0;
}
