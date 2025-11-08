"use client";

import React from "react";
import {
  Card,
  CardContent,
  Typography,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow,
  Paper,
  Box,
  Chip,
} from "@mui/material";
import { Metrics } from "@/app/types/metrics";
import {
  formatLatency,
  formatMemory,
  formatThroughput,
  formatNumber,
} from "@/app/utils/formatters";
import { RB_TREE_COLOR, SKIP_LIST_COLOR } from "@/app/theme";

interface PerformanceComparisonProps {
  rbTree?: Metrics;
  skipList?: Metrics;
}

interface ComparisonRow {
  label: string;
  rbTreeValue: number | string;
  skipListValue: number | string;
  formatValue?: (value: number) => string;
  lowerIsBetter?: boolean;
}

export default function PerformanceComparison({
  rbTree,
  skipList,
}: PerformanceComparisonProps) {
  if (!rbTree || !skipList) {
    return null;
  }

  const getWinner = (
    rbValue: number,
    skipValue: number,
    lowerIsBetter: boolean = true
  ): "rb" | "skip" | "tie" => {
    if (rbValue === skipValue) return "tie";
    if (lowerIsBetter) {
      return rbValue < skipValue ? "rb" : "skip";
    } else {
      return rbValue > skipValue ? "rb" : "skip";
    }
  };

  const rows: ComparisonRow[] = [
    {
      label: "Avg Insert Time",
      rbTreeValue: rbTree.avgInsertTime,
      skipListValue: skipList.avgInsertTime,
      formatValue: formatLatency,
      lowerIsBetter: true,
    },
    {
      label: "Avg Delete Time",
      rbTreeValue: rbTree.avgDeleteTime,
      skipListValue: skipList.avgDeleteTime,
      formatValue: formatLatency,
      lowerIsBetter: true,
    },
    {
      label: "Avg Lookup Time",
      rbTreeValue: rbTree.avgLookupTime,
      skipListValue: skipList.avgLookupTime,
      formatValue: formatLatency,
      lowerIsBetter: true,
    },
    {
      label: "Min Latency",
      rbTreeValue: rbTree.minLatency,
      skipListValue: skipList.minLatency,
      formatValue: formatLatency,
      lowerIsBetter: true,
    },
    {
      label: "Max Latency",
      rbTreeValue: rbTree.maxLatency,
      skipListValue: skipList.maxLatency,
      formatValue: formatLatency,
      lowerIsBetter: true,
    },
    {
      label: "Median Latency",
      rbTreeValue: rbTree.medianLatency,
      skipListValue: skipList.medianLatency,
      formatValue: formatLatency,
      lowerIsBetter: true,
    },
    {
      label: "95th Percentile",
      rbTreeValue: rbTree.percentile95,
      skipListValue: skipList.percentile95,
      formatValue: formatLatency,
      lowerIsBetter: true,
    },
    {
      label: "99th Percentile",
      rbTreeValue: rbTree.percentile99,
      skipListValue: skipList.percentile99,
      formatValue: formatLatency,
      lowerIsBetter: true,
    },
    {
      label: "Orders Per Second",
      rbTreeValue: rbTree.ordersPerSecond,
      skipListValue: skipList.ordersPerSecond,
      formatValue: formatThroughput,
      lowerIsBetter: false,
    },
    {
      label: "Total Orders",
      rbTreeValue: rbTree.totalOrders,
      skipListValue: skipList.totalOrders,
      formatValue: formatNumber,
      lowerIsBetter: false,
    },
    {
      label: "Memory Usage",
      rbTreeValue: rbTree.memoryUsageBytes,
      skipListValue: skipList.memoryUsageBytes,
      formatValue: formatMemory,
      lowerIsBetter: true,
    },
    {
      label: "Add Count",
      rbTreeValue: rbTree.addCount,
      skipListValue: skipList.addCount,
      formatValue: formatNumber,
      lowerIsBetter: false,
    },
    {
      label: "Cancel Count",
      rbTreeValue: rbTree.cancelCount,
      skipListValue: skipList.cancelCount,
      formatValue: formatNumber,
      lowerIsBetter: false,
    },
    {
      label: "Modify Count",
      rbTreeValue: rbTree.modifyCount,
      skipListValue: skipList.modifyCount,
      formatValue: formatNumber,
      lowerIsBetter: false,
    },
  ];

  return (
    <Box>
      <Typography variant="h6" component="h3" gutterBottom sx={{ mb: 2 }}>
        Performance Comparison
      </Typography>
      <Card>
        <CardContent>
          <TableContainer component={Paper} variant="outlined">
            <Table>
              <TableHead>
                <TableRow>
                  <TableCell>
                    <strong>Metric</strong>
                  </TableCell>
                  <TableCell align="right">
                    <strong>Red-Black Tree</strong>
                  </TableCell>
                  <TableCell align="right">
                    <strong>Skip List</strong>
                  </TableCell>
                  <TableCell align="center">
                    <strong>Winner</strong>
                  </TableCell>
                </TableRow>
              </TableHead>
              <TableBody>
                {rows.map((row) => {
                  const rbValue = row.rbTreeValue as number;
                  const skipValue = row.skipListValue as number;
                  const winner = getWinner(
                    rbValue,
                    skipValue,
                    row.lowerIsBetter
                  );
                  const formatFunc = row.formatValue || ((v: number) => String(v));

                  return (
                    <TableRow key={row.label}>
                      <TableCell component="th" scope="row">
                        {row.label}
                      </TableCell>
                      <TableCell
                        align="right"
                        sx={{
                          backgroundColor:
                            winner === "rb"
                              ? `${RB_TREE_COLOR}20`
                              : "transparent",
                        }}
                      >
                        {formatFunc(rbValue)}
                      </TableCell>
                      <TableCell
                        align="right"
                        sx={{
                          backgroundColor:
                            winner === "skip"
                              ? `${SKIP_LIST_COLOR}20`
                              : "transparent",
                        }}
                      >
                        {formatFunc(skipValue)}
                      </TableCell>
                      <TableCell align="center">
                        {winner === "tie" ? (
                          <Chip label="Tie" size="small" color="default" />
                        ) : winner === "rb" ? (
                          <Chip
                            label="RB Tree"
                            size="small"
                            sx={{ backgroundColor: RB_TREE_COLOR, color: "white" }}
                          />
                        ) : (
                          <Chip
                            label="Skip List"
                            size="small"
                            sx={{
                              backgroundColor: SKIP_LIST_COLOR,
                              color: "white",
                            }}
                          />
                        )}
                      </TableCell>
                    </TableRow>
                  );
                })}
              </TableBody>
            </Table>
          </TableContainer>
        </CardContent>
      </Card>
    </Box>
  );
}

