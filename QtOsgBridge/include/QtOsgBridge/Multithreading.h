#pragma once

#include <QApplication>
#include <functional>

namespace QtOsgBridge
{

/*!
 *  \class Multithreading
 *  This class provides static functions that ensure that code is executed in the UI thread
 *  The Qt application has to inherit this class to work properly
 */
class Multithreading : public QApplication
{
  Q_OBJECT

public:
  /*!
   *  The constructor
   *  \param argc The number of arguments
   *  \param argv The command line arguments
   */
  Multithreading(int& argc, char** argv);
  virtual ~Multithreading();

  /*!
   *  Executes a function in UI thread. Blocks the current thread until the execution
   *  of the function is finished
   *  \param func The function to execute
   */
  static void executeInUiBlocking(std::function<void()> func);

  /*!
   *  Executes a function in UI thread. The function is executed asynchronously
   *  This function should not be called from UI thread
   *  \param func              The function to execute
   *  \param executeInUIThread true if this function in executed in UI thread
   */
  static void executeInUiAsync(std::function<void()> func, bool executeInUIThread = false);

  //! \returns true if this function is called from UI thread
  static bool isInUIThread();

Q_SIGNALS:
  void executeFunctionAsync(std::function<void()> func);
  void executeFunctionBlocking(std::function<void()> func);

private Q_SLOTS:
  void executeFunction(std::function<void()> func) const;
};

}  // namespace QtOsgBridge