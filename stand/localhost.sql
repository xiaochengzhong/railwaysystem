-- phpMyAdmin SQL Dump
-- version 3.4.9
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2012 年 03 月 16 日 19:09
-- 服务器版本: 5.5.20
-- PHP 版本: 5.3.10

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- 数据库: `railwaysystem`
--
CREATE DATABASE `railwaysystem` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `railwaysystem`;

-- --------------------------------------------------------

--
-- 表的结构 `orders`
--

CREATE TABLE IF NOT EXISTS `orders` (
  `Oid` int(11) NOT NULL AUTO_INCREMENT,
  `Uid` int(11) NOT NULL,
  `Tid` int(11) NOT NULL,
  `DepartureDate` date NOT NULL,
  `Num` int(11) NOT NULL,
  `Price` int(11) NOT NULL,
  `RiseTime` datetime NOT NULL,
  `State` int(11) NOT NULL,
  PRIMARY KEY (`Oid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=5 ;

--
-- 转存表中的数据 `orders`
--

INSERT INTO `orders` (`Oid`, `Uid`, `Tid`, `DepartureDate`, `Num`, `Price`, `RiseTime`, `State`) VALUES
(1, 8, 5, '2012-03-21', 2, 394, '2012-03-16 13:49:11', 2),
(2, 8, 3, '2012-03-21', 4, 788, '2012-03-16 15:53:47', 3),
(3, 9, 7, '2012-03-24', 1, 197, '2012-03-16 16:19:53', 0),
(4, 0, 5, '2012-03-29', 3, 591, '2012-03-16 18:56:03', 2);

-- --------------------------------------------------------

--
-- 表的结构 `ticket`
--

CREATE TABLE IF NOT EXISTS `ticket` (
  `Tid` int(11) NOT NULL,
  `DepartureDate` date NOT NULL,
  `Price` int(11) NOT NULL,
  `Num` int(11) NOT NULL,
  PRIMARY KEY (`Tid`,`DepartureDate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `ticket`
--

INSERT INTO `ticket` (`Tid`, `DepartureDate`, `Price`, `Num`) VALUES
(1, '2012-03-19', 157, 400),
(1, '2012-03-20', 157, 400),
(1, '2012-03-21', 157, 400),
(1, '2012-03-22', 157, 400),
(1, '2012-03-23', 157, 400),
(1, '2012-03-24', 157, 400),
(1, '2012-03-25', 157, 400),
(1, '2012-03-26', 157, 400),
(1, '2012-03-27', 157, 400),
(1, '2012-03-28', 157, 400),
(1, '2012-03-29', 157, 400),
(1, '2012-03-30', 157, 400),
(1, '2012-03-31', 157, 400),
(2, '2012-03-19', 97, 350),
(2, '2012-03-20', 97, 350),
(2, '2012-03-21', 97, 350),
(2, '2012-03-22', 97, 350),
(2, '2012-03-23', 97, 350),
(2, '2012-03-24', 97, 350),
(2, '2012-03-25', 97, 350),
(2, '2012-03-26', 97, 350),
(2, '2012-03-27', 97, 350),
(2, '2012-03-28', 97, 350),
(2, '2012-03-29', 97, 350),
(2, '2012-03-30', 97, 350),
(2, '2012-03-31', 97, 350),
(3, '2012-03-19', 197, 350),
(3, '2012-03-20', 197, 322),
(3, '2012-03-21', 197, 31),
(3, '2012-03-22', 197, 50),
(3, '2012-03-23', 197, 430),
(3, '2012-03-24', 197, 354),
(3, '2012-03-25', 197, 320),
(3, '2012-03-26', 197, 350),
(3, '2012-03-27', 197, 50),
(3, '2012-03-28', 197, 350),
(3, '2012-03-29', 197, 30),
(3, '2012-03-30', 197, 350),
(3, '2012-03-31', 197, 300),
(4, '2012-03-19', 292, 350),
(4, '2012-03-20', 292, 350),
(4, '2012-03-21', 292, 350),
(4, '2012-03-22', 292, 350),
(4, '2012-03-23', 292, 350),
(4, '2012-03-24', 292, 350),
(4, '2012-03-25', 292, 350),
(4, '2012-03-26', 292, 350),
(4, '2012-03-27', 292, 350),
(4, '2012-03-28', 292, 350),
(4, '2012-03-29', 292, 350),
(4, '2012-03-30', 292, 350),
(4, '2012-03-31', 292, 350),
(5, '2012-03-19', 197, 350),
(5, '2012-03-20', 197, 322),
(5, '2012-03-21', 197, 33),
(5, '2012-03-22', 197, 50),
(5, '2012-03-23', 197, 430),
(5, '2012-03-24', 197, 354),
(5, '2012-03-25', 197, 320),
(5, '2012-03-26', 197, 350),
(5, '2012-03-27', 197, 50),
(5, '2012-03-28', 197, 350),
(5, '2012-03-29', 197, 27),
(5, '2012-03-30', 197, 350),
(5, '2012-03-31', 197, 300),
(6, '2012-03-19', 292, 350),
(6, '2012-03-20', 292, 350),
(6, '2012-03-21', 292, 350),
(6, '2012-03-22', 292, 350),
(6, '2012-03-23', 292, 350),
(6, '2012-03-24', 292, 350),
(6, '2012-03-25', 292, 350),
(6, '2012-03-26', 292, 350),
(6, '2012-03-27', 292, 350),
(6, '2012-03-28', 292, 350),
(6, '2012-03-29', 292, 350),
(6, '2012-03-30', 292, 350),
(6, '2012-03-31', 292, 350),
(7, '2012-03-19', 197, 350),
(7, '2012-03-20', 197, 322),
(7, '2012-03-21', 197, 35),
(7, '2012-03-22', 197, 50),
(7, '2012-03-23', 197, 430),
(7, '2012-03-24', 197, 353),
(7, '2012-03-25', 197, 320),
(7, '2012-03-26', 197, 350),
(7, '2012-03-27', 197, 50),
(7, '2012-03-28', 197, 350),
(7, '2012-03-29', 197, 30),
(7, '2012-03-30', 197, 350),
(7, '2012-03-31', 197, 300);

-- --------------------------------------------------------

--
-- 表的结构 `traininfo`
--

CREATE TABLE IF NOT EXISTS `traininfo` (
  `Tid` int(11) NOT NULL AUTO_INCREMENT,
  `Tnum` varchar(20) NOT NULL,
  `Originator` varchar(20) NOT NULL,
  `Terminal` varchar(20) NOT NULL,
  `DepartureTime` time NOT NULL,
  `ArrivalTime` time NOT NULL,
  PRIMARY KEY (`Tid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=8 ;

--
-- 转存表中的数据 `traininfo`
--

INSERT INTO `traininfo` (`Tid`, `Tnum`, `Originator`, `Terminal`, `DepartureTime`, `ArrivalTime`) VALUES
(1, 'k25', '南京', '深圳', '15:00:00', '10:00:00'),
(2, 'K26', '深圳', '南京', '15:00:00', '10:00:00'),
(3, 'T66', '南京', '北京', '19:00:00', '08:00:00'),
(4, 'G121', '南京', '北京', '09:00:00', '14:00:00'),
(5, 'G2421', '南京', '上海', '12:00:00', '13:34:00'),
(6, 'G5423', '南京', '上海', '15:30:00', '17:00:00'),
(7, 'G1414', '南京', '上海', '08:10:00', '09:54:00');

-- --------------------------------------------------------

--
-- 表的结构 `userinfo`
--

CREATE TABLE IF NOT EXISTS `userinfo` (
  `Uid` int(11) NOT NULL AUTO_INCREMENT,
  `Uname` varchar(20) NOT NULL,
  `Pwd` varchar(20) NOT NULL,
  `Sex` varchar(10) NOT NULL,
  `PhoneNum` int(11) NOT NULL,
  `Email` varchar(20) NOT NULL,
  PRIMARY KEY (`Uid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=14 ;

--
-- 转存表中的数据 `userinfo`
--

INSERT INTO `userinfo` (`Uid`, `Uname`, `Pwd`, `Sex`, `PhoneNum`, `Email`) VALUES
(8, 'zxc', 'zxc', '男', 123123, 'zxc@qq.com'),
(9, 'zs', 'zs', '男', 1234567890, 'zs@zs.com'),
(10, 'jy', 'jy', '男', 123123, 'jy@jy.com'),
(13, 'jz', 'jz', '男', 123456, 'jz@jz.com');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
