#ifndef PROVIDER_H
#define PROVIDER_H

#include <QObject>
#include <QHash>
#include <QVariant>

typedef QHash<int, QVariant> ProviderParam;

class iProvider : public QObject
{
  Q_OBJECT
public:
  explicit iProvider(QObject *parent = 0): QObject(parent) {}

  QString name() const { return m_providerName; }
  void setName(const QString& strName) { m_providerName = strName; }

  QVariant param(int field) { return m_params[field]; }
  void setParam(int field, QVariant value) { m_params[field] = value; emit paramChanged(field, value); }
  void setParams(const ProviderParam& params) { m_params = params; emit paramsChanged(m_params); }

  virtual void work(int id) = 0;

private:
  QString m_providerName;

protected:
  ProviderParam m_params;

signals:
  void paramsChanged(ProviderParam params);
  void paramChanged(int param, QVariant value);

  void dataReceived(int, QByteArray);
  void done(int);
  void finished(int);
};

#define iProvider_iid "com.plexusdynamics.framework.scraping.providers.iProvider"
Q_DECLARE_INTERFACE(iProvider, iProvider_iid)


#endif // PROVIDER_H
