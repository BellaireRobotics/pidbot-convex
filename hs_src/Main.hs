{-# LANGUAGE ForeignFunctionInterface #-}
module Main where

import Foreign
--import Foreign.C
import Foreign.C.Types
import Data.Int

foreign import ccall unsafe "vexMotorSet" c_vexMotorSet :: Int -> Int -> IO ()
vexMotorSet :: Int -> Int -> IO ()
vexMotorSet m s = c_vexMotorSet m s

main :: IO ()
main = vexMotorSet 1 60