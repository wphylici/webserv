/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wphylici <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 22:30:19 by wphylici          #+#    #+#             */
/*   Updated: 2021/06/23 09:11:08 by wphylici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main ()
{
	int i = 0;
	int j = 0;

	Server start;
	start.ParseConf();

	std::cout << start.getServInfo()[i]->getValueLocPath()[j] << std::endl;

	for (int i = 0; i <= start.getPosServ(); i++)  // <--- Этот кусок кода использовать
		delete start.getServInfo()[i];			   // при завершении всей программы

	return (0);
}
