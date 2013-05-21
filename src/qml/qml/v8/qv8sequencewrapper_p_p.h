/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QV8SEQUENCEWRAPPER_P_P_H
#define QV8SEQUENCEWRAPPER_P_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qqmlengine_p.h>
#include <private/qqmlmetatype_p.h>

#include <private/qv4arrayobject_p.h>

QT_BEGIN_NAMESPACE

/*!
  \internal
  \class QV8SequenceResource
  \brief The abstract base class of the external resource used in sequence type objects

  Every sequence type object returned by QV8SequenceWrapper::fromVariant() or
  QV8SequenceWrapper::newSequence() has a type-specific QV8SequenceResource which
  contains the type name, the meta type ids of the sequence and sequence element
  types, as well as either the sequence data (copy) or object pointer and property
  index (reference) data associated with the sequence.
 */
class QV8SequenceResource : public QV8ObjectResource
{
    V8_RESOURCE_TYPE(SequenceType);

public:
    virtual ~QV8SequenceResource() {}

    enum ObjectType { Reference, Copy };

    virtual QVariant toVariant() = 0;
    virtual bool isEqual(const QV8SequenceResource *v) = 0;

    virtual quint32 lengthGetter() = 0;
    virtual void lengthSetter(v8::Handle<v8::Value> value) = 0;
    virtual v8::Handle<v8::Value> indexedSetter(quint32 index, v8::Handle<v8::Value> value) = 0;
    virtual v8::Handle<v8::Value> indexedGetter(quint32 index) = 0;
    virtual v8::Handle<v8::Value> indexedDeleter(quint32 index) = 0;
    virtual v8::Handle<v8::Array> indexedEnumerator() = 0;
    virtual QV4::Value toString() = 0;
    virtual void sort(v8::Handle<v8::Function> comparer) = 0;

    ObjectType objectType;
    QByteArray typeName;
    int sequenceMetaTypeId;
    int elementMetaTypeId;

protected:
    QV8SequenceResource(QV8Engine *engine, ObjectType type, const QByteArray &name, int sequenceId, int elementId)
        : QV8ObjectResource(engine), objectType(type), typeName(name), sequenceMetaTypeId(sequenceId), elementMetaTypeId(elementId)
    {
    }
};

// helper function to generate valid warnings if errors occur during sequence operations.
static void generateWarning(QV8Engine *engine, const QString& description)
{
    if (!engine)
        return;
    v8::Handle<v8::StackTrace> currStack = v8::StackTrace::CurrentStackTrace(1);
    if (currStack.IsEmpty())
        return;
    v8::Handle<v8::StackFrame> currFrame = currStack->GetFrame(0);
    if (currFrame.IsEmpty())
        return;

    QQmlError retn;
    retn.setDescription(description);
    retn.setLine(currFrame->GetLineNumber());
    retn.setUrl(QUrl(currFrame->GetScriptName()->v4Value().toQString()));
    QQmlEnginePrivate::warning(engine->engine(), retn);
}


static int convertV8ValueToInt(QV8Engine *, v8::Handle<v8::Value> v)
{
    return v->Int32Value();
}

static v8::Handle<v8::Value> convertIntToV8Value(QV8Engine *, int v)
{
    return QV4::Value::fromInt32(v);
}

static QString convertIntToString(QV8Engine *, int v)
{
    return QString::number(v);
}

static qreal convertV8ValueToReal(QV8Engine *, v8::Handle<v8::Value> v)
{
    return v->NumberValue();
}

static v8::Handle<v8::Value> convertRealToV8Value(QV8Engine *, qreal v)
{
    return QV4::Value::fromDouble(v);
}

static QString convertRealToString(QV8Engine *, qreal v)
{
    return QString::number(v);
}

static bool convertV8ValueToBool(QV8Engine *, v8::Handle<v8::Value> v)
{
    return v->BooleanValue();
}

static v8::Handle<v8::Value> convertBoolToV8Value(QV8Engine *, bool v)
{
    return QV4::Value::fromBoolean(v);
}

static QString convertBoolToString(QV8Engine *, bool v)
{
    if (v)
        return QLatin1String("true");
    return QLatin1String("false");
}

static QString convertV8ValueToString(QV8Engine *e, v8::Handle<v8::Value> v)
{
    return v->v4Value().toQString();
}

static v8::Handle<v8::Value> convertStringToV8Value(QV8Engine *e, const QString &v)
{
    return e->toString(v);
}

static QString convertStringToString(QV8Engine *, const QString &v)
{
    return v;
}

static QString convertV8ValueToQString(QV8Engine *e, v8::Handle<v8::Value> v)
{
    return v->v4Value().toQString();
}

static v8::Handle<v8::Value> convertQStringToV8Value(QV8Engine *e, const QString &v)
{
    return e->toString(v);
}

static QString convertQStringToString(QV8Engine *, const QString &v)
{
    return v;
}

static QUrl convertV8ValueToUrl(QV8Engine *e, v8::Handle<v8::Value> v)
{
    return QUrl(v->v4Value().toQString());
}

static v8::Handle<v8::Value> convertUrlToV8Value(QV8Engine *e, const QUrl &v)
{
    return e->toString(QLatin1String(v.toEncoded().data()));
}

static QString convertUrlToString(QV8Engine *, const QUrl &v)
{
    return v.toString();
}


/*
  \internal
  \class QV8<Type>SequenceResource
  \brief The external resource used in sequence type objects

  Every sequence type object returned by QV8SequenceWrapper::newSequence() has
  a QV8<Type>SequenceResource which contains a property index and a pointer
  to the object which contains the property.

  Every sequence type object returned by QV8SequenceWrapper::fromVariant() has
  a QV8<Type>SequenceResource which contains a copy of the sequence value.
  Operations on the sequence are implemented directly in terms of that sequence data.

  There exists one QV8<Type>SequenceResource instance for every JavaScript Object
  (sequence) instance returned from QV8SequenceWrapper::newSequence() or
  QV8SequenceWrapper::fromVariant().
 */

//  F(elementType, elementTypeName, sequenceType, defaultValue)
#define FOREACH_QML_SEQUENCE_TYPE(F) \
    F(int, Int, QList<int>, 0) \
    F(qreal, Real, QList<qreal>, 0.0) \
    F(bool, Bool, QList<bool>, false) \
    F(QString, String, QList<QString>, QString()) \
    F(QUrl, Url, QList<QUrl>, QUrl())

#define QML_SEQUENCE_TYPE_RESOURCE(SequenceElementType, SequenceElementTypeName, SequenceType, DefaultValue, ConversionToV8fn, ConversionFromV8fn, ToStringfn) \
    QT_END_NAMESPACE \
    Q_DECLARE_METATYPE(SequenceType) \
    QT_BEGIN_NAMESPACE \
    class QV8##SequenceElementTypeName##SequenceResource : public QV8SequenceResource { \
        public:\
            QV8##SequenceElementTypeName##SequenceResource(QV8Engine *engine, QObject *obj, int propIdx) \
                : QV8SequenceResource(engine, QV8SequenceResource::Reference, #SequenceType, qMetaTypeId<SequenceType>(), qMetaTypeId<SequenceElementType>()) \
                , object(obj), propertyIndex(propIdx) \
            { \
            } \
            QV8##SequenceElementTypeName##SequenceResource(QV8Engine *engine, const SequenceType &value) \
                : QV8SequenceResource(engine, QV8SequenceResource::Copy, #SequenceType, qMetaTypeId<SequenceType>(), qMetaTypeId<SequenceElementType>()) \
                , object(0), propertyIndex(-1), c(value) \
            { \
            } \
            ~QV8##SequenceElementTypeName##SequenceResource() \
            { \
            } \
            static QVariant toVariant(QV8Engine *e, v8::Handle<v8::Array> array, uint32_t length, bool *succeeded) \
            { \
                SequenceType list; \
                list.reserve(length); \
                for (uint32_t ii = 0; ii < length; ++ii) { \
                    list.append(ConversionFromV8fn(e, array->Get(ii))); \
                } \
                *succeeded = true; \
                return QVariant::fromValue<SequenceType>(list); \
            } \
            QVariant toVariant() \
            { \
                if (objectType == QV8SequenceResource::Reference) { \
                    if (!object) \
                        return QVariant(); \
                    loadReference(); \
                } \
                return QVariant::fromValue<SequenceType>(c); \
            } \
            bool isEqual(const QV8SequenceResource *v) \
            { \
                /* Note: two different sequences can never be equal (even if they  */ \
                /* contain the same elements in the same order) in order to        */ \
                /* maintain JavaScript semantics.  However, if they both reference */ \
                /* the same QObject+propertyIndex, they are equal.                 */ \
                if (objectType == QV8SequenceResource::Reference && v->objectType == QV8SequenceResource::Reference) { \
                    if (sequenceMetaTypeId == v->sequenceMetaTypeId) { \
                        const QV8##SequenceElementTypeName##SequenceResource *rhs = static_cast<const QV8##SequenceElementTypeName##SequenceResource *>(v); \
                        return (object != 0 && object == rhs->object && propertyIndex == rhs->propertyIndex); \
                    } \
                } else if (objectType == QV8SequenceResource::Copy && v->objectType == QV8SequenceResource::Copy) { \
                    if (sequenceMetaTypeId == v->sequenceMetaTypeId) { \
                        const QV8##SequenceElementTypeName##SequenceResource *rhs = static_cast<const QV8##SequenceElementTypeName##SequenceResource *>(v); \
                        return (this == rhs); \
                    } \
                } \
                return false; \
            } \
            quint32 lengthGetter() \
            { \
                if (objectType == QV8SequenceResource::Reference) { \
                    if (!object) \
                        return 0; \
                    loadReference(); \
                } \
                return static_cast<quint32>(c.count()); \
            } \
            void lengthSetter(v8::Handle<v8::Value> value) \
            { \
                /* Get the new required length */ \
                if (value.IsEmpty() || !value->IsUint32()) \
                    return; \
                quint32 newLength = value->Uint32Value(); \
                /* Qt containers have int (rather than uint) allowable indexes. */ \
                if (newLength > INT_MAX) { \
                    generateWarning(engine, QLatin1String("Index out of range during length set")); \
                    return; \
                } \
                /* Read the sequence from the QObject property if we're a reference */ \
                if (objectType == QV8SequenceResource::Reference) { \
                    if (!object) \
                        return; \
                    loadReference(); \
                } \
                /* Determine whether we need to modify the sequence */ \
                qint32 newCount = static_cast<qint32>(newLength); \
                qint32 count = c.count(); \
                if (newCount == count) { \
                    return; \
                } else if (newCount > count) { \
                    /* according to ECMA262r3 we need to insert */ \
                    /* undefined values increasing length to newLength. */ \
                    /* We cannot, so we insert default-values instead. */ \
                    c.reserve(newCount); \
                    while (newCount > count++) { \
                        c.append(DefaultValue); \
                    } \
                } else { \
                    /* according to ECMA262r3 we need to remove */ \
                    /* elements until the sequence is the required length. */ \
                    while (newCount < count) { \
                        count--; \
                        c.removeAt(count); \
                    } \
                } \
                /* write back if required. */ \
                if (objectType == QV8SequenceResource::Reference) { \
                    /* write back.  already checked that object is non-null, so skip that check here. */ \
                    storeReference(); \
                } \
                return; \
            } \
            v8::Handle<v8::Value> indexedSetter(quint32 index, v8::Handle<v8::Value> value) \
            { \
                /* Qt containers have int (rather than uint) allowable indexes. */ \
                if (index > INT_MAX) { \
                    generateWarning(engine, QLatin1String("Index out of range during indexed set")); \
                    return QV4::Value::undefinedValue(); \
                } \
                if (objectType == QV8SequenceResource::Reference) { \
                    if (!object) \
                        return QV4::Value::undefinedValue(); \
                    loadReference(); \
                } \
                /* modify the sequence */ \
                SequenceElementType elementValue = ConversionFromV8fn(engine, value); \
                qint32 count = c.count(); \
                qint32 signedIdx = static_cast<qint32>(index); \
                if (signedIdx == count) { \
                    c.append(elementValue); \
                } else if (signedIdx < count) { \
                    c[index] = elementValue; \
                } else { \
                    /* according to ECMA262r3 we need to insert */ \
                    /* the value at the given index, increasing length to index+1. */ \
                    c.reserve(signedIdx + 1); \
                    while (signedIdx > count++) { \
                        c.append(DefaultValue); \
                    } \
                    c.append(elementValue); \
                } \
                /* write back.  already checked that object is non-null, so skip that check here. */ \
                if (objectType == QV8SequenceResource::Reference) \
                    storeReference(); \
                return value; \
            } \
            v8::Handle<v8::Value> indexedGetter(quint32 index) \
            { \
                /* Qt containers have int (rather than uint) allowable indexes. */ \
                if (index > INT_MAX) { \
                    generateWarning(engine, QLatin1String("Index out of range during indexed get")); \
                    return QV4::Value::undefinedValue(); \
                } \
                if (objectType == QV8SequenceResource::Reference) { \
                    if (!object) \
                        return QV4::Value::undefinedValue(); \
                    loadReference(); \
                } \
                qint32 count = c.count(); \
                qint32 signedIdx = static_cast<qint32>(index); \
                if (signedIdx < count) \
                    return ConversionToV8fn(engine, c.at(signedIdx)); \
                return QV4::Value::undefinedValue(); \
            } \
            v8::Handle<v8::Value> indexedDeleter(quint32 index) \
            { \
                /* Qt containers have int (rather than uint) allowable indexes. */ \
                if (index > INT_MAX) \
                    return QV4::Value::fromBoolean(false); \
                /* Read in the sequence from the QObject */ \
                if (objectType == QV8SequenceResource::Reference) { \
                    if (!object) \
                        return QV4::Value::fromBoolean(false); \
                    loadReference(); \
                } \
                qint32 signedIdx = static_cast<qint32>(index); \
                if (signedIdx < c.count()) { \
                    /* according to ECMA262r3 it should be Undefined, */ \
                    /* but we cannot, so we insert a default-value instead. */ \
                    c.replace(signedIdx, DefaultValue); \
                    if (objectType == QV8SequenceResource::Reference) { \
                        /* write back.  already checked that object is non-null, so skip that check here. */ \
                        storeReference(); \
                    } \
                    return QV4::Value::fromBoolean(true); \
                } \
                return QV4::Value::fromBoolean(false); \
            } \
            v8::Handle<v8::Array> indexedEnumerator() \
            { \
                if (objectType == QV8SequenceResource::Reference) { \
                    if (!object) \
                        return v8::Handle<v8::Array>(); \
                    loadReference(); \
                } \
                qint32 count = c.count(); \
                v8::Handle<v8::Array> retn = v8::Array::New(count); \
                for (qint32 i = 0; i < count; ++i) { \
                    retn->Set(static_cast<quint32>(i), QV4::Value::fromUInt32(static_cast<quint32>(i))); \
                } \
                return retn; \
            } \
            QV4::Value toString() \
            { \
                if (objectType == QV8SequenceResource::Reference) { \
                    if (!object) \
                        return QV4::Value::undefinedValue(); \
                    loadReference(); \
                } \
                QString str; \
                qint32 count = c.count(); \
                for (qint32 i = 0; i < count; ++i) { \
                    str += QString(QLatin1String("%1,")).arg(ToStringfn(engine, c[i])); \
                } \
                str.chop(1); \
                return engine->toString(str); \
            } \
            void loadReference() \
            { \
                Q_ASSERT(object); \
                Q_ASSERT(objectType == QV8SequenceResource::Reference); \
                void *a[] = { &c, 0 }; \
                QMetaObject::metacall(object, QMetaObject::ReadProperty, propertyIndex, a); \
            } \
            void storeReference() \
            { \
                Q_ASSERT(object); \
                Q_ASSERT(objectType == QV8SequenceResource::Reference); \
                int status = -1; \
                QQmlPropertyPrivate::WriteFlags flags = \
                    QQmlPropertyPrivate::DontRemoveBinding; \
                void *a[] = { &c, 0, &status, &flags }; \
                QMetaObject::metacall(object, QMetaObject::WriteProperty, propertyIndex, a); \
            } \
            class CompareFunctor \
            { \
            public: \
                CompareFunctor(QV8Engine *engine, v8::Handle<v8::Function> f) : jsFn(f), eng(engine) {} \
                bool operator()(SequenceElementType e0, SequenceElementType e1) \
                { \
                    v8::Handle<v8::Value> argv[2] = { eng->fromVariant(e0), eng->fromVariant(e1) }; \
                    v8::Handle<v8::Value> compareValue = jsFn->Call(v8::Value::fromV4Value(eng->global()), 2, argv); \
                    return compareValue->NumberValue() < 0; \
                } \
            private: \
                v8::Handle<v8::Function> jsFn; \
                QV8Engine *eng; \
            }; \
            void sort(v8::Handle<v8::Function> jsCompareFunction) \
            { \
                CompareFunctor cf(engine, jsCompareFunction); \
                qSort(c.begin(), c.end(), cf); \
            } \
        private: \
            QQmlGuard<QObject> object; \
            int propertyIndex; \
            SequenceType c; \
    };

#define GENERATE_QML_SEQUENCE_TYPE_RESOURCE(ElementType, ElementTypeName, SequenceType, DefaultValue) \
    QML_SEQUENCE_TYPE_RESOURCE(ElementType, ElementTypeName, SequenceType, DefaultValue, convert##ElementTypeName##ToV8Value, convertV8ValueTo##ElementTypeName, convert##ElementTypeName##ToString)

FOREACH_QML_SEQUENCE_TYPE(GENERATE_QML_SEQUENCE_TYPE_RESOURCE)
#undef GENERATE_QML_SEQUENCE_TYPE_RESOURCE
#undef QML_SEQUENCE_TYPE_RESOURCE

class QQmlStringList : public QV4::Object
{
public:
    QQmlStringList(QV4::ExecutionEngine *engine, const QStringList &container)
        : QV4::Object(engine)
        , m_container(container)
    {
        type = Type_QmlStringList;
        vtbl = &static_vtbl;
        m_lengthProperty = insertMember(engine->id_length, QV4::Attr_ReadOnly);
        prototype = engine->arrayPrototype;
        updateLength();
    }

    QQmlStringList(QV4::ExecutionEngine *engine, QObject *object, int propertyIndex)
        : QV4::Object(engine)
    {
        type = Type_QmlStringList;
        vtbl = &static_vtbl;
        m_lengthProperty = insertMember(engine->id_length, QV4::Attr_ReadOnly);
        prototype = engine->arrayPrototype;
        void *a[] = { &m_container, 0 };
        QMetaObject::metacall(object, QMetaObject::ReadProperty, propertyIndex, a);
        updateLength();
    }

    QV4::Value containerGetIndexed(QV4::ExecutionContext *ctx, uint index, bool *hasProperty)
    {
        /* Qt containers have int (rather than uint) allowable indexes. */
        if (index > INT_MAX) {
            generateWarning(QV8Engine::get(ctx->engine->publicEngine), QLatin1String("Index out of range during indexed get"));
            if (hasProperty)
                *hasProperty = false;
            return QV4::Value::undefinedValue();
        }
        qint32 signedIdx = static_cast<qint32>(index);
        if (signedIdx < m_container.count()) {
            if (hasProperty)
                *hasProperty = true;
            return QV4::Value::fromString(ctx, m_container.at(signedIdx));
        }
        if (hasProperty)
            *hasProperty = false;
        return QV4::Value::undefinedValue();
    }

    void containerPutIndexed(QV4::ExecutionContext *ctx, uint index, const QV4::Value &value)
    {
        /* Qt containers have int (rather than uint) allowable indexes. */
        if (index > INT_MAX) {
            generateWarning(QV8Engine::get(ctx->engine->publicEngine), QLatin1String("Index out of range during indexed put"));
            return;
        }
        qint32 signedIdx = static_cast<qint32>(index);

        int count = m_container.count();

        QString element = value.toQString();

        if (signedIdx == count) {
            m_container.append(element);
            updateLength();
        } else if (signedIdx < count) {
            m_container[signedIdx] = element;
        } else {
            /* according to ECMA262r3 we need to insert */
            /* the value at the given index, increasing length to index+1. */
            m_container.reserve(signedIdx + 1);
            while (signedIdx > count++) {
                m_container.append(QString());
            }
            m_container.append(element);
            updateLength();
        }
    }

    QV4::PropertyAttributes containerQueryIndexed(QV4::ExecutionContext *ctx, uint index)
    {
        /* Qt containers have int (rather than uint) allowable indexes. */
        if (index > INT_MAX) {
            generateWarning(QV8Engine::get(ctx->engine->publicEngine), QLatin1String("Index out of range during indexed query"));
            return QV4::Attr_Invalid;
        }
        qint32 signedIdx = static_cast<qint32>(index);
        return (index < m_container.count()) ? QV4::Attr_Data : QV4::Attr_Invalid;
    }

    bool containerDeleteIndexedProperty(QV4::ExecutionContext *ctx, uint index)
    {
        /* Qt containers have int (rather than uint) allowable indexes. */
        if (index > INT_MAX)
            return false;
        qint32 signedIdx = static_cast<qint32>(index);

        if (signedIdx >= m_container.count())
            return false;

        /* according to ECMA262r3 it should be Undefined, */
        /* but we cannot, so we insert a default-value instead. */
        m_container.replace(signedIdx, QString());
        return true;
    }

    QVariant toVariant() const
    { return QVariant::fromValue<QStringList>(m_container); }

private:
    void updateLength()
    {
        m_lengthProperty->value = QV4::Value::fromInt32(m_container.length());
    }

    QStringList m_container;
    QV4::Property *m_lengthProperty;

    static QV4::Value getIndexed(QV4::Managed *that, QV4::ExecutionContext *ctx, uint index, bool *hasProperty)
    { return static_cast<QQmlStringList *>(that)->containerGetIndexed(ctx, index, hasProperty); }
    static void putIndexed(Managed *that, QV4::ExecutionContext *ctx, uint index, const QV4::Value &value)
    { static_cast<QQmlStringList *>(that)->containerPutIndexed(ctx, index, value); }
    static QV4::PropertyAttributes queryIndexed(QV4::Managed *that, QV4::ExecutionContext *ctx, uint index)
    { return static_cast<QQmlStringList *>(that)->containerQueryIndexed(ctx, index); }
    static bool deleteIndexedProperty(QV4::Managed *that, QV4::ExecutionContext *ctx, uint index)
    { return static_cast<QQmlStringList *>(that)->containerDeleteIndexedProperty(ctx, index); }

    static void destroy(Managed *that)
    {
        static_cast<QQmlStringList *>(that)->~QQmlStringList();
    }

    static const QV4::ManagedVTable static_vtbl;
};

DEFINE_MANAGED_VTABLE(QQmlStringList);

QT_END_NAMESPACE

#endif // QV8SEQUENCEWRAPPER_P_P_H
