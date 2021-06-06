#include <QTimer>
#include <zlib.h>

#include "rlottie.h"
#include "rlottieimage.h"

#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

LottieHandler::LottieHandler()
{

}

LottieHandler::~LottieHandler()
{

}

// code yoinked from https://stackoverflow.com/questions/2690328/qt-quncompress-gzip-data
auto decompress(QByteArray& input, QByteArray &output) -> bool
{
    // Prepare output
    output.clear();

    // Is there something to do?
    if(input.length() > 0)
    {
        // Prepare inflater status
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        // Initialize inflater
        int ret = inflateInit2(&strm, GZIP_WINDOWS_BIT);

        if (ret != Z_OK)
            return(false);

        // Extract pointer to input data
        char *input_data = input.data();
        int input_data_left = input.length();

        // Decompress data until available
        do {
            // Determine current chunk size
            int chunk_size = qMin(GZIP_CHUNK_SIZE, input_data_left);

            // Check for termination
            if(chunk_size <= 0)
                break;

            // Set inflater references
            strm.next_in = (unsigned char*)input_data;
            strm.avail_in = chunk_size;

            // Update interval variables
            input_data += chunk_size;
            input_data_left -= chunk_size;

            // Inflate chunk and cumulate output
            do {

                // Declare vars
                char out[GZIP_CHUNK_SIZE];

                // Set inflater references
                strm.next_out = (unsigned char*)out;
                strm.avail_out = GZIP_CHUNK_SIZE;

                // Try to inflate chunk
                ret = inflate(&strm, Z_NO_FLUSH);

                switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                case Z_STREAM_ERROR:
                    // Clean-up
                    inflateEnd(&strm);

                    // Return
                    return(false);
                }

                // Determine decompressed size
                int have = (GZIP_CHUNK_SIZE - strm.avail_out);

                // Cumulate result
                if(have > 0)
                    output.append((char*)out, have);

            } while (strm.avail_out == 0);

        } while (ret != Z_STREAM_END);

        // Clean-up
        inflateEnd(&strm);

        // Return
        return (ret == Z_STREAM_END);
    }
    else
        return(true);
}

bool LottieHandler::load(QIODevice* it) const
{
    if (loaded) {
        return true;
    }

    auto dat = it->readAll();
    QByteArray uncompressed;
    if (!decompress(dat, uncompressed)) {
        return false;
    }

    auto data = uncompressed.toStdString();
    auto key = QString::number(qHash(uncompressed)).toStdString();

    animation.reset(rlottie::Animation::loadFromData(data, key).release());
    loaded = true;
    imageData.reserve(animation->totalFrame());

    if (!mmappedFile.open()) {
        return false;
    }
    if (!mmappedFile.resize(animation->totalFrame()*512*512*4)) {
        return false;
    }
    auto base = mmappedFile.map(0, animation->totalFrame()*512*512*4);
    if (base == nullptr) {
        return false;
    }

    for (int i = 0; i < animation->totalFrame(); i++) {
        auto data = base+(i*512*512*4);
        rlottie::Surface surface((uint32_t*)data, 512, 512, 512*4);

        qDebug() << "rendering...";
        animation->renderSync(i, surface);
        imageData << (QRgb*)data;
    }

    return true;
}

bool LottieHandler::read(QImage *image)
{
    if (!loaded) {
        if (!load(device())) {
            qWarning() << "failed to read";
            return false;
        }
    }

    currentFrame++;
    if (currentFrame >= animation->totalFrame()) {
        currentFrame = 0;
    }

    QImage out((uchar*)imageData[currentFrame], 512, 512, QImage::Format_ARGB32_Premultiplied);
    *image = out;

    return true;
}

bool LottieHandler::canRead() const
{
    if (device() == nullptr) {
        qWarning() << "cannot read (nullptr device)";
        return false;
    }

    return format() == "lottie" or format() == "tgs";
}

QVariant LottieHandler::option(ImageOption option) const
{
    switch (option) {
    case ImageOption::Size:
        return QSize(512, 512);
    default:
        return false;
    }
}

void LottieHandler::setOption(ImageOption option, const QVariant &value)
{
    Q_UNUSED(option)
    Q_UNUSED(value)
}

bool LottieHandler::supportsOption(ImageOption option) const
{
    switch (option) {
    case ImageOption::Size:
        return true;
    default:
        return false;
    }
}


int LottieHandler::imageCount() const
{
    if (!loaded) {
        if (!load(device())) {
            return -1;
        }
    }

    return animation->totalFrame();
}

int LottieHandler::loopCount() const
{
    return std::numeric_limits<int>::max();
}

int LottieHandler::nextImageDelay() const
{
    return (1/animation->frameRate())*1000;
}

int LottieHandler::currentImageNumber() const
{
    return currentFrame;
}

