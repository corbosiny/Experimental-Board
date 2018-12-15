
"""Reads in data from various file formats"""

from typing import List, Dict
import io
import xml.etree.ElementTree as et

def read_rock_sim(filename: str,
                  convert_to_kilos: bool = True) -> List[Dict[str, float]]:
    """Reads in a rock_sim data file

    Args:
        filename: Location of .rse file

    Returns:
        Iterator of data points from the file

    Raises:
        OSError: the file cannot be opened
    """
    with io.open(filename, 'r') as file:
        root = et.fromstring(file.read())

        # Assumes that there's only one engine in the file
        engine_database = root

        engine_list = engine_database[0]
        engine = engine_list[0]
        data = engine[1 if len(engine) >= 2 and
            engine[0].tag == 'comments' else 0]

    # Iterate through each eng-data tag and convert the values to floats
    return list(map(lambda x: dict(
        map(lambda y: (y[0], (float(y[1]) if y[0] != 'm' and convert_to_kilos else 0.001 * float(y[1]))), x.attrib.items())), data))
