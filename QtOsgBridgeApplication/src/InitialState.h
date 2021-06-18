#pragma once

#include <QtOsgBridge/EventProcessingState.h>

#include <osgHelper/ppu/HDR.h>
#include <osgHelper/ppu/FXAA.h>
#include <osgHelper/ppu/DOF.h>

#include <QPointer>
#include <QWidget>

class State1 : public QtOsgBridge::EventProcessingState
{
public:
  State1(osgHelper::ioc::Injector& injector);
  ~State1() override;

  void onInitialize(QPointer<QtOsgBridge::MainWindow> mainWindow, const SimulationData& data) override;
  void onExit() override;

private:
  QPointer<QWidget> m_overlay;

};

class State2 : public QtOsgBridge::EventProcessingState
{
public:
  State2(osgHelper::ioc::Injector& injector);
  ~State2() override;

  void onInitialize(QPointer<QtOsgBridge::MainWindow> mainWindow, const SimulationData& data) override;
  void onExit() override;

private:
  QPointer<QWidget> m_overlay;

};

class InitialState : public QtOsgBridge::EventProcessingState
{
  Q_OBJECT

public:
  InitialState(osgHelper::ioc::Injector& injector);
  ~InitialState() override;

  void onInitialize(QPointer<QtOsgBridge::MainWindow> mainWindow, const SimulationData& data) override;
  void onExit() override;
  void onUpdate(const SimulationData& data) override;

protected:
  bool onMouseEvent(QMouseEvent* event) override;
  bool onKeyEvent(QKeyEvent* event) override;

private:
  osg::ref_ptr<osgHelper::ppu::FXAA> m_fxaa;
  osg::ref_ptr<osgHelper::ppu::DOF>  m_dof;
  osg::ref_ptr<osgHelper::ppu::HDR>  m_hdr;

  float  m_rotation;
  double m_position;
  float  m_overlayAlpha;

  QPointer<QtOsgBridge::MainWindow>     m_mainWindow;
  QPointer<QWidget>                     m_overlay;
  QPointer<QtOsgBridge::VirtualOverlay> m_virtualOverlay;

  osg::ref_ptr<osg::PositionAttitudeTransform> m_boxTransform;

  void updateTransformation();

};