-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Nov 23, 2022 at 06:18 PM
-- Server version: 10.4.24-MariaDB
-- PHP Version: 8.1.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `basicrp`
--

-- --------------------------------------------------------

--
-- Table structure for table `accounts`
--

CREATE TABLE `accounts` (
  `ID` int(11) NOT NULL,
  `Username` varchar(25) NOT NULL DEFAULT '',
  `Password` varchar(129) NOT NULL DEFAULT '',
  `Salt` varchar(65) NOT NULL DEFAULT '',
  `IP` varchar(17) NOT NULL DEFAULT '127.0.0.1',
  `LeaveIP` varchar(17) NOT NULL DEFAULT '0.0.0.0',
  `Banned` int(4) NOT NULL DEFAULT 0,
  `Admin` int(4) NOT NULL DEFAULT 0,
  `AdminDuty` int(4) NOT NULL DEFAULT 0,
  `AdminHide` int(4) NOT NULL DEFAULT 0,
  `RegisterDate` int(32) NOT NULL DEFAULT 0,
  `LoginDate` int(32) NOT NULL DEFAULT 0,
  `AdminRankName` varchar(32) NOT NULL DEFAULT 'None',
  `ReportPoint` int(11) NOT NULL DEFAULT 0,
  `AdminDutyTime` int(11) UNSIGNED NOT NULL DEFAULT 0,
  `AdminAcceptReport` int(11) NOT NULL DEFAULT 0,
  `AdminDeniedReport` int(11) NOT NULL DEFAULT 0,
  `AdminAcceptStuck` int(11) NOT NULL DEFAULT 0,
  `AdminDeniedStuck` int(11) NOT NULL DEFAULT 0,
  `AdminBanned` int(11) NOT NULL DEFAULT 0,
  `AdminUnbanned` int(11) NOT NULL DEFAULT 0,
  `AdminJail` int(11) NOT NULL DEFAULT 0,
  `AdminAnswer` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `characters`
--

CREATE TABLE `characters` (
  `ID` int(12) NOT NULL,
  `Username` varchar(24) NOT NULL DEFAULT '',
  `Character` varchar(24) NOT NULL DEFAULT '',
  `Created` int(4) NOT NULL DEFAULT 0,
  `Gender` int(4) NOT NULL DEFAULT 0,
  `Birthdate` varchar(32) NOT NULL DEFAULT '01/01/1970',
  `Origin` varchar(32) NOT NULL DEFAULT 'Not Specified',
  `Skin` int(12) NOT NULL DEFAULT 0,
  `PosX` float NOT NULL DEFAULT 0,
  `PosY` float NOT NULL DEFAULT 0,
  `PosZ` float NOT NULL DEFAULT 0,
  `PosA` float NOT NULL DEFAULT 0,
  `Interior` int(12) NOT NULL DEFAULT 0,
  `World` int(12) NOT NULL DEFAULT 0,
  `Money` int(12) NOT NULL DEFAULT 500,
  `BankMoney` int(12) NOT NULL DEFAULT 1000,
  `Minutes` int(12) NOT NULL DEFAULT 0,
  `Health` float NOT NULL DEFAULT 0,
  `MaskID` int(12) NOT NULL DEFAULT 0,
  `AdminHide` int(4) NOT NULL DEFAULT 0,
  `SpawnPoint` int(11) NOT NULL DEFAULT 0,
  `pScore` int(11) NOT NULL DEFAULT 1,
  `Played` varchar(64) NOT NULL DEFAULT '',
  `Alias` varchar(24) NOT NULL DEFAULT 'Player',
  `LoginDate` varchar(50) NOT NULL DEFAULT '',
  `RegisterDate` varchar(50) NOT NULL DEFAULT '0',
  `CreateDate` int(12) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `accounts`
--
ALTER TABLE `accounts`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `Username` (`Username`);

--
-- Indexes for table `characters`
--
ALTER TABLE `characters`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `accounts`
--
ALTER TABLE `accounts`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `characters`
--
ALTER TABLE `characters`
  MODIFY `ID` int(12) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
