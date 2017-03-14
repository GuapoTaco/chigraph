#pragma once

#ifndef CHIGGUI_FUNCTIONS_PANE_HPP
#define CHIGGUI_FUNCTIONS_PANE_HPP

#include <QListWidget>

#include <chig/GraphModule.hpp>

class MainWindow;

class FunctionsPane : public QListWidget {
	Q_OBJECT
public:
	explicit FunctionsPane(QWidget* parent = nullptr);

public slots:

	void updateModule(chig::GraphModule& mod);

private slots:

	void selectItem(QListWidgetItem* newitem);

signals:

	void functionSelected(chig::GraphFunction* funcs);
};

#endif  // CHIGGUI_FUNCTIONS_PANE_HPP
