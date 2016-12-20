# Global configuration for all demo projects. Please run
#  git update-index --skip-worktree config.mk
# after modifications if you plan on contributing. This way the remote
# repository will not contain your changes.

# Directory for STM32CUBEF0, containing the necessary CMSIS headers and
# startup sources
CUBE_DIR ?= /opt/stm32cube/STM32Cube_FW_F0_V1.6.0

# If no local configuration is present, generate a debug build
DEBUG ?= yes
