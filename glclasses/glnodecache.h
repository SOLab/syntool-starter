#ifndef GLNODECACHE_H
#define GLNODECACHE_H

#include <QObject>
#include <QHash>
#include <QList>

template <class Key, class T>
class GLNodeCache : public QObject
{
public:
    GLNodeCache(QObject *parent = 0);

    inline qint32 maxCost() const { return maxCostValue; }
    void setMaxCost(int m);
    inline qint32 totalCost() const { return totalCostValue; }

    inline int size() const { return hash.size(); }
    inline int count() const { return hash.size(); }
    inline bool isEmpty() const { return hash.isEmpty(); }
    inline QList<Key> keys() const { return hash.keys(); }

    void clear();

    bool insert(const Key &key, T *object, int cost = 1);
    T *object(const Key &key);
    inline bool contains(const Key &key) const { return hash.contains(key); }
    T *operator[](const Key &key) const;

    bool remove(const Key &key);
    T *take(const Key &key);

protected:
    QHash<Key, T*> hash;
    QList<Key> granuleNumberList;
    qint32 maxCostValue;
    qint32 totalCostValue;
};

template <class Key, class T>
inline bool GLNodeCache<Key,T>::insert(const Key &key, T *object, int cost)
{
    remove(key);

    // release the required number
    while(granuleNumberList.size() + cost > maxCostValue)
    {
        Key removeKey = granuleNumberList.takeFirst();
        remove(removeKey);
    }

    // remove if exists and append
    granuleNumberList.append(key);
    qCritical() << "insert ===========" << granuleNumberList.size();
    hash.insert(key, object);
    return true;
}

template <class Key, class T>
GLNodeCache<Key, T>::GLNodeCache(QObject *parent) :
    QObject(parent)
{
}

template <class Key, class T>
void GLNodeCache<Key,T>::setMaxCost(int m)
{maxCostValue = m;}

template <class Key, class T>
T *GLNodeCache<Key,T>::object(const Key &key)
{
    granuleNumberList.removeAll(key);
    granuleNumberList.append(key);
    qCritical() << "object ===========" << granuleNumberList.size();
    return hash.value(key);
}

template <class Key, class T>
T *GLNodeCache<Key,T>::operator[](const Key &key) const
{ return object(key); }

template <class Key, class T>
bool GLNodeCache<Key,T>::remove(const Key &key)
{
    if (granuleNumberList.contains(key))
        granuleNumberList.removeAll(key);

    if (hash.contains(key))
    {
        T *obj = hash.take(key);
        qCritical() << "remove ===========" << granuleNumberList.size();
//        obj->palette()->material(obj->materialIndex())->texture()->cleanupResources();
//        delete obj->palette()->material(obj->materialIndex())->texture();
        delete obj;
        return true;
    }
    qCritical() << "remove ===========" << granuleNumberList.size();
    return false;
}

template <class Key, class T>
void GLNodeCache<Key,T>::clear()
{
    granuleNumberList.clear();
    hash.clear();
}

template <class Key, class T>
T *GLNodeCache<Key,T>::take(const Key &key)
{
    T *obj = object(key);
    remove(key);
    qCritical() << "take ===========" << granuleNumberList.size();
    return obj;
}


#endif // GLNODECACHE_H
