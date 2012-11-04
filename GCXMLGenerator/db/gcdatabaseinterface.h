#ifndef GCDATABASEINTERFACE_H
#define GCDATABASEINTERFACE_H

#include <QObject>
#include <QMap>
#include <QtSql/QSqlQuery>

class QDomDocument;

class GCDataBaseInterface : public QObject
{
  Q_OBJECT
public:
  explicit GCDataBaseInterface( QObject *parent = 0 );

  /* Read in the list of known DB connections and add them to the SQLite driver.  This
    function must be called shortly after an instance of this object is created. */
  bool initialise();

  /* Processes an entire DOM document, adding new or updating existing elements (with their
    corresponding comments and associated attributes) and known attribute values. */
  bool batchProcessDOMDocument( const QDomDocument &domDoc ) const;

  /* Adds a single new element (this function does nothing if an element with the same name
    already exists). */
  bool addElement( const QString &element, const QStringList &comments, const QStringList &attributes ) const;

  /* Updates the list of known comments associated with "element" by appending
    the new comments to the existing list (nothing is deleted). */
  bool updateElementComments  ( const QString &element, const QStringList &comments ) const;

  /* Updates the list of known attributes associated with "element" by appending
    the new attributes to the existing list (nothing is deleted). */
  bool updateElementAttributes( const QString &element, const QStringList &attributes ) const;

  /* Updates the list of known attribute values that is associated with "element" and its
    corresponding "attribute" by appending the new attribute values to the existing list
    (nothing is deleted). */
  bool updateAttributeValues  ( const QString &element, const QString &attribute, const QStringList &attributeValues ) const;

  /* Remove single entries/values from the database. */
  bool removeElement          ( const QString &element ) const;
  bool removeElementComment   ( const QString &element, const QString &comment ) const;
  bool removeElementAttribute ( const QString &element, const QString &attribute ) const;
  bool removeAttributeValue   ( const QString &element, const QString &attribute, const QString &attributeValue ) const;

  /* Getters. */
  QStringList getDBList() const;    // returns a list of known DB connections
  QString  getLastError() const;    // returns the last error message set
  bool hasActiveSession() const;    // returns "true" if an active DB session exists, "false" if not

  /* Returns a sorted (case sensitive, ascending) list of all the element names known to
    the current database connection (the active session). */
  QStringList knownElements() const;

  /* Returns a sorted (case sensitive, ascending) list of all the attributes associated with
    "element" in the active database. */
  QStringList attributes     ( const QString &element, bool &success ) const;

  /* Returns a sorted (case sensitive, ascending) list of all the attribute values associated with
    "element" and its corresponding "attribute" in the active database. */
  QStringList attributeValues( const QString &element, const QString &attribute, bool &success ) const;
  
public slots:
  bool addDatabase   ( QString dbName );
  bool removeDatabase( QString dbName );
  bool setSessionDB  ( QString dbName );

private:
  QSqlQuery   selectElement  ( const QString &element, bool &success ) const;
  QSqlQuery   selectAttribute( const QString &element, const QString &attribute, bool &success ) const;
  QStringList knownAttributes() const;

  bool openDBConnection( QString dbConName, QString &errMsg ) const;
  bool initialiseDB    ( QString dbConName, QString &errMsg ) const;
  void saveDBFile() const;

  QString                  m_sessionDBName;
  mutable QString          m_lastErrorMsg;
  bool                     m_hasActiveSession;
  QMap< QString /*connection name*/, QString /*file name*/ > m_dbMap;
};

#endif // GCDATABASEINTERFACE_H
