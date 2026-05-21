#ifndef SIMULATIONSELECTIONDIALOG_H
#define SIMULATIONSELECTIONDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>

class SimulationSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimulationSelectionDialog(QWidget *parent = nullptr);

private:
    QVBoxLayout* mMainLayout;
    QHBoxLayout* mButtonsLayout;
    QLabel* mLabel;
    QPushButton* m1DButton;
    QPushButton* m2DButton;
    QPushButton* mCancelButton;

signals:
    void simulation1DButtonClicked();
    void simulation2DButtonClicked();
    void simulationCancelClicked();

private slots:
    void simulation1DButtonPressed();
    void simulation2DButtonPressed();
    void simulationCancelPressed();
};

#endif // SIMULATIONSELECTIONDIALOG_H
