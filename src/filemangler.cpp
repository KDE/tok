// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "filemangler_p.h"

enum Roles {
    FileSize,
    ExpectedFileSize,

    LocalFilePath,
    LocalFileDownloadable,
    LocalFileDeletable,
    LocalFileIsDownloading,
    LocalFileDownloadCompleted,
    LocalFileDownloadedSize,
};

FileMangler::FileMangler(Client* parent) : TokAbstractRelationalModel(parent), c(parent), d(new Private)
{
    connect(parent, &Client::fileDataChanged, this, [this](qint32 ID, QSharedPointer<TDApi::file> file) {
        d->fileData[ID] = file;
        Q_EMIT keyDataChanged(QString::number(file->id_), {});
    });
}

FileMangler::~FileMangler()
{

}

auto to(const std::string& it) { return QString::fromStdString(it); }

QVariant FileMangler::data(const QVariant& key, int role)
{
    if (!checkKey(key)) {
        return QVariant();
    }

    auto id = key.toString().toLong();
    const auto& data = d->fileData[id];

    switch (Roles(role)) {
    case Roles::FileSize: {
        return data->size_;
    }
    case Roles::ExpectedFileSize: {
        return data->expected_size_;
    }
    case Roles::LocalFilePath: {
        return to(data->local_->path_);
    }
    case Roles::LocalFileDownloadable: {
        return data->local_->can_be_downloaded_;
    }
    case Roles::LocalFileDeletable: {
        return data->local_->can_be_deleted_;
    }
    case Roles::LocalFileIsDownloading: {
        return data->local_->is_downloading_active_;
    }
    case Roles::LocalFileDownloadCompleted: {
        return data->local_->is_downloading_completed_;
    }
    case Roles::LocalFileDownloadedSize: {
        return data->local_->downloaded_size_;
    }
    }

}

bool FileMangler::checkKey(const QVariant& key)
{
    return d->fileData.contains(key.toString().toLong());
}

bool FileMangler::canFetchKey(const QVariant& key)
{
    Q_UNUSED(key)

    return true;
}

void FileMangler::fetchKey(const QVariant& key)
{
    auto id = key.toString().toLong();
    c->call<TDApi::getFile>([id, this](TDApi::getFile::ReturnType it) {
        d->fileData[id] = QSharedPointer<TDApi::file>(it.release());
        Q_EMIT keyDataChanged(QString::number(id), {});
    }, id);
}

QHash<int,QByteArray> FileMangler::roleNames()
{
    return {
        { FileSize, "mFileSize" },
        { ExpectedFileSize, "mExpectedFileSize" },
        { LocalFilePath, "mLocalFilePath" },
        { LocalFileDownloadable, "mLocalFileDownloadable" },
        { LocalFileDeletable, "mLocalFileDeletable" },
        { LocalFileIsDownloading, "mLocalFileIsDownloading" },
        { LocalFileDownloadCompleted, "mLocalFileDownloadCompleted" },
        { LocalFileDownloadedSize, "mLocalFileDownloadedSize" },
    };
}

void FileMangler::stopDownloadingFile(const QString &id)
{
    c->call<TDApi::cancelDownloadFile>(nullptr, id.toLong(), false);
}

QIviPendingReplyBase FileMangler::downloadFile(const QString& id)
{
    QIviPendingReply<QString> reply;

    auto it = id.toLong();

    auto onFinished = [this, reply, downloadingID = it](qint32 id, QSharedPointer<TDApi::file> file) mutable {
        d->fileData[id] = file;
        Q_EMIT keyDataChanged(QString::number(file->id_), {});

        if (id != downloadingID) {
            return;
        }

        if (!file->local_->is_downloading_completed_) {
            return;
        }

        reply.setSuccess(to(file->local_->path_));
    };

    c->call<TDApi::downloadFile>(
        [onFinished](TDApi::downloadFile::ReturnType t) mutable {
            auto id = t->id_;
            onFinished(id, QSharedPointer<TDApi::file>(t.release()));
        },
        it, 10, 0, 0, false
    );

    return reply;
}
