//
// Generated file, do not edit! Created by opp_msgtool 6.3 from src/Packet.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "Packet_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(IoTPacket)

IoTPacket::IoTPacket(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

IoTPacket::IoTPacket(const IoTPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

IoTPacket::~IoTPacket()
{
}

IoTPacket& IoTPacket::operator=(const IoTPacket& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void IoTPacket::copy(const IoTPacket& other)
{
    this->sourceId = other.sourceId;
    this->destinationId = other.destinationId;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->temperatureValue = other.temperatureValue;
    this->nonce = other.nonce;
    this->payloadSize = other.payloadSize;
    this->encryptionAlgorithm = other.encryptionAlgorithm;
    this->authenticationTag = other.authenticationTag;
    this->isEncrypted_ = other.isEncrypted_;
    this->isTampered_ = other.isTampered_;
    this->isReplay_ = other.isReplay_;
    this->isAccepted_ = other.isAccepted_;
    this->attackType = other.attackType;
}

void IoTPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sourceId);
    doParsimPacking(b,this->destinationId);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->temperatureValue);
    doParsimPacking(b,this->nonce);
    doParsimPacking(b,this->payloadSize);
    doParsimPacking(b,this->encryptionAlgorithm);
    doParsimPacking(b,this->authenticationTag);
    doParsimPacking(b,this->isEncrypted_);
    doParsimPacking(b,this->isTampered_);
    doParsimPacking(b,this->isReplay_);
    doParsimPacking(b,this->isAccepted_);
    doParsimPacking(b,this->attackType);
}

void IoTPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceId);
    doParsimUnpacking(b,this->destinationId);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->temperatureValue);
    doParsimUnpacking(b,this->nonce);
    doParsimUnpacking(b,this->payloadSize);
    doParsimUnpacking(b,this->encryptionAlgorithm);
    doParsimUnpacking(b,this->authenticationTag);
    doParsimUnpacking(b,this->isEncrypted_);
    doParsimUnpacking(b,this->isTampered_);
    doParsimUnpacking(b,this->isReplay_);
    doParsimUnpacking(b,this->isAccepted_);
    doParsimUnpacking(b,this->attackType);
}

const char * IoTPacket::getSourceId() const
{
    return this->sourceId.c_str();
}

void IoTPacket::setSourceId(const char * sourceId)
{
    this->sourceId = sourceId;
}

const char * IoTPacket::getDestinationId() const
{
    return this->destinationId.c_str();
}

void IoTPacket::setDestinationId(const char * destinationId)
{
    this->destinationId = destinationId;
}

int IoTPacket::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void IoTPacket::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

omnetpp::simtime_t IoTPacket::getTimestamp() const
{
    return this->timestamp;
}

void IoTPacket::setTimestamp(omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

double IoTPacket::getTemperatureValue() const
{
    return this->temperatureValue;
}

void IoTPacket::setTemperatureValue(double temperatureValue)
{
    this->temperatureValue = temperatureValue;
}

const char * IoTPacket::getNonce() const
{
    return this->nonce.c_str();
}

void IoTPacket::setNonce(const char * nonce)
{
    this->nonce = nonce;
}

int IoTPacket::getPayloadSize() const
{
    return this->payloadSize;
}

void IoTPacket::setPayloadSize(int payloadSize)
{
    this->payloadSize = payloadSize;
}

const char * IoTPacket::getEncryptionAlgorithm() const
{
    return this->encryptionAlgorithm.c_str();
}

void IoTPacket::setEncryptionAlgorithm(const char * encryptionAlgorithm)
{
    this->encryptionAlgorithm = encryptionAlgorithm;
}

const char * IoTPacket::getAuthenticationTag() const
{
    return this->authenticationTag.c_str();
}

void IoTPacket::setAuthenticationTag(const char * authenticationTag)
{
    this->authenticationTag = authenticationTag;
}

bool IoTPacket::isEncrypted() const
{
    return this->isEncrypted_;
}

void IoTPacket::setIsEncrypted(bool isEncrypted)
{
    this->isEncrypted_ = isEncrypted;
}

bool IoTPacket::isTampered() const
{
    return this->isTampered_;
}

void IoTPacket::setIsTampered(bool isTampered)
{
    this->isTampered_ = isTampered;
}

bool IoTPacket::isReplay() const
{
    return this->isReplay_;
}

void IoTPacket::setIsReplay(bool isReplay)
{
    this->isReplay_ = isReplay;
}

bool IoTPacket::isAccepted() const
{
    return this->isAccepted_;
}

void IoTPacket::setIsAccepted(bool isAccepted)
{
    this->isAccepted_ = isAccepted;
}

const char * IoTPacket::getAttackType() const
{
    return this->attackType.c_str();
}

void IoTPacket::setAttackType(const char * attackType)
{
    this->attackType = attackType;
}

class IoTPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_sourceId,
        FIELD_destinationId,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_temperatureValue,
        FIELD_nonce,
        FIELD_payloadSize,
        FIELD_encryptionAlgorithm,
        FIELD_authenticationTag,
        FIELD_isEncrypted,
        FIELD_isTampered,
        FIELD_isReplay,
        FIELD_isAccepted,
        FIELD_attackType,
    };
  public:
    IoTPacketDescriptor();
    virtual ~IoTPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(IoTPacketDescriptor)

IoTPacketDescriptor::IoTPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(IoTPacket)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

IoTPacketDescriptor::~IoTPacketDescriptor()
{
    delete[] propertyNames;
}

bool IoTPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IoTPacket *>(obj)!=nullptr;
}

const char **IoTPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *IoTPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int IoTPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 14+base->getFieldCount() : 14;
}

unsigned int IoTPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_sourceId
        FD_ISEDITABLE,    // FIELD_destinationId
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_temperatureValue
        FD_ISEDITABLE,    // FIELD_nonce
        FD_ISEDITABLE,    // FIELD_payloadSize
        FD_ISEDITABLE,    // FIELD_encryptionAlgorithm
        FD_ISEDITABLE,    // FIELD_authenticationTag
        FD_ISEDITABLE,    // FIELD_isEncrypted
        FD_ISEDITABLE,    // FIELD_isTampered
        FD_ISEDITABLE,    // FIELD_isReplay
        FD_ISEDITABLE,    // FIELD_isAccepted
        FD_ISEDITABLE,    // FIELD_attackType
    };
    return (field >= 0 && field < 14) ? fieldTypeFlags[field] : 0;
}

const char *IoTPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceId",
        "destinationId",
        "sequenceNumber",
        "timestamp",
        "temperatureValue",
        "nonce",
        "payloadSize",
        "encryptionAlgorithm",
        "authenticationTag",
        "isEncrypted",
        "isTampered",
        "isReplay",
        "isAccepted",
        "attackType",
    };
    return (field >= 0 && field < 14) ? fieldNames[field] : nullptr;
}

int IoTPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "sourceId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destinationId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "temperatureValue") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "nonce") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "payloadSize") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "encryptionAlgorithm") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "authenticationTag") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "isEncrypted") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "isTampered") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "isReplay") == 0) return baseIndex + 11;
    if (strcmp(fieldName, "isAccepted") == 0) return baseIndex + 12;
    if (strcmp(fieldName, "attackType") == 0) return baseIndex + 13;
    return base ? base->findField(fieldName) : -1;
}

const char *IoTPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_sourceId
        "string",    // FIELD_destinationId
        "int",    // FIELD_sequenceNumber
        "omnetpp::simtime_t",    // FIELD_timestamp
        "double",    // FIELD_temperatureValue
        "string",    // FIELD_nonce
        "int",    // FIELD_payloadSize
        "string",    // FIELD_encryptionAlgorithm
        "string",    // FIELD_authenticationTag
        "bool",    // FIELD_isEncrypted
        "bool",    // FIELD_isTampered
        "bool",    // FIELD_isReplay
        "bool",    // FIELD_isAccepted
        "string",    // FIELD_attackType
    };
    return (field >= 0 && field < 14) ? fieldTypeStrings[field] : nullptr;
}

const char **IoTPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IoTPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IoTPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void IoTPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'IoTPacket'", field);
    }
}

const char *IoTPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IoTPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: return oppstring2string(pp->getSourceId());
        case FIELD_destinationId: return oppstring2string(pp->getDestinationId());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return simtime2string(pp->getTimestamp());
        case FIELD_temperatureValue: return double2string(pp->getTemperatureValue());
        case FIELD_nonce: return oppstring2string(pp->getNonce());
        case FIELD_payloadSize: return long2string(pp->getPayloadSize());
        case FIELD_encryptionAlgorithm: return oppstring2string(pp->getEncryptionAlgorithm());
        case FIELD_authenticationTag: return oppstring2string(pp->getAuthenticationTag());
        case FIELD_isEncrypted: return bool2string(pp->isEncrypted());
        case FIELD_isTampered: return bool2string(pp->isTampered());
        case FIELD_isReplay: return bool2string(pp->isReplay());
        case FIELD_isAccepted: return bool2string(pp->isAccepted());
        case FIELD_attackType: return oppstring2string(pp->getAttackType());
        default: return "";
    }
}

void IoTPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: pp->setSourceId((value)); break;
        case FIELD_destinationId: pp->setDestinationId((value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2simtime(value)); break;
        case FIELD_temperatureValue: pp->setTemperatureValue(string2double(value)); break;
        case FIELD_nonce: pp->setNonce((value)); break;
        case FIELD_payloadSize: pp->setPayloadSize(string2long(value)); break;
        case FIELD_encryptionAlgorithm: pp->setEncryptionAlgorithm((value)); break;
        case FIELD_authenticationTag: pp->setAuthenticationTag((value)); break;
        case FIELD_isEncrypted: pp->setIsEncrypted(string2bool(value)); break;
        case FIELD_isTampered: pp->setIsTampered(string2bool(value)); break;
        case FIELD_isReplay: pp->setIsReplay(string2bool(value)); break;
        case FIELD_isAccepted: pp->setIsAccepted(string2bool(value)); break;
        case FIELD_attackType: pp->setAttackType((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'IoTPacket'", field);
    }
}

omnetpp::cValue IoTPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: return pp->getSourceId();
        case FIELD_destinationId: return pp->getDestinationId();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp().dbl();
        case FIELD_temperatureValue: return pp->getTemperatureValue();
        case FIELD_nonce: return pp->getNonce();
        case FIELD_payloadSize: return pp->getPayloadSize();
        case FIELD_encryptionAlgorithm: return pp->getEncryptionAlgorithm();
        case FIELD_authenticationTag: return pp->getAuthenticationTag();
        case FIELD_isEncrypted: return pp->isEncrypted();
        case FIELD_isTampered: return pp->isTampered();
        case FIELD_isReplay: return pp->isReplay();
        case FIELD_isAccepted: return pp->isAccepted();
        case FIELD_attackType: return pp->getAttackType();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'IoTPacket' as cValue -- field index out of range?", field);
    }
}

void IoTPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: pp->setSourceId(value.stringValue()); break;
        case FIELD_destinationId: pp->setDestinationId(value.stringValue()); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_temperatureValue: pp->setTemperatureValue(value.doubleValue()); break;
        case FIELD_nonce: pp->setNonce(value.stringValue()); break;
        case FIELD_payloadSize: pp->setPayloadSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_encryptionAlgorithm: pp->setEncryptionAlgorithm(value.stringValue()); break;
        case FIELD_authenticationTag: pp->setAuthenticationTag(value.stringValue()); break;
        case FIELD_isEncrypted: pp->setIsEncrypted(value.boolValue()); break;
        case FIELD_isTampered: pp->setIsTampered(value.boolValue()); break;
        case FIELD_isReplay: pp->setIsReplay(value.boolValue()); break;
        case FIELD_isAccepted: pp->setIsAccepted(value.boolValue()); break;
        case FIELD_attackType: pp->setAttackType(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'IoTPacket'", field);
    }
}

const char *IoTPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr IoTPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void IoTPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    IoTPacket *pp = omnetpp::fromAnyPtr<IoTPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'IoTPacket'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

