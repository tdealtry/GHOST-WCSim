from hkpilot.utils.cmake import CMake
from hkpilot.utils import fancylogger
import os

logger = fancylogger.getLogger(__name__)


class GHOST_WCSim(CMake):

    def __init__(self, path):
        super().__init__(path)
        self._package_name = "GHOST-WCSim"

    def post_install(self):
        # Run standard post_install step
        super().post_install()

        # Link compiled tool into
        compiled_tools_folder = os.environ.get("HK_COMPILEDTOOLS_DIR")
        logger.info(f"Linking installation to {compiled_tools_folder}")
        if not os.path.exists(compiled_tools_folder):
            logger.debug("Folder doesn't exist: creating it")
            os.mkdir(compiled_tools_folder)
        symlink = os.path.join(compiled_tools_folder, self._package_name)
        if not os.path.islink(symlink):
            os.symlink(self._install_folder, symlink)
        return True
